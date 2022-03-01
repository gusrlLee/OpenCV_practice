#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

// feature matches method
void find_feature_matches (
    const cv::Mat& img_1, const cv::Mat& img_2,
    std::vector<KeyPoint>& keypoints_1,
    std::vector<KeyPoint>& keypoints_2,
    std::vector< DMatch >& matches );

// estimation 2d 2d
void pose_estimation_2d2d (
    const std::vector<KeyPoint>& keypoints_1,
    const std::vector<KeyPoint>& keypoints_2,
    const std::vector< DMatch >& matches,
    Mat& R, Mat& t );

// triangulation calculate
void triangulation (
    const vector<KeyPoint>& keypoint_1,
    const vector<KeyPoint>& keypoint_2,
    const std::vector< DMatch >& matches,
    const Mat& R, const Mat& t,
    vector<Point3d>& points
);

// pixel -> camera coordinate system
Point2f pixel2cam( const Point2d& p, const Mat& K );

// MAIN METHOD!!
int main(int argc, char** argv){

    // image load
    cv::Mat img_1 = cv::imread("../../../data/1.png");
    cv::Mat img_2 = cv::imread("../../../data/2.png");

    // find Keypoints in each image 
    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    std::vector<cv::DMatch> matches;
    // goto find_feature_matches method
    find_feature_matches(img_1, img_2, keypoints_1, keypoints_2, matches);
    // save features in match vector and print
    std::cout << "match size " << matches.size() << std::endl;

    // Define Rotation Matrix and Translation Matrix
    cv::Mat R,t;
    // goto pose_estimation_2d2d
    pose_estimation_2d2d(keypoints_1, keypoints_2, matches, R, t);

    // project 3D space 
    std::vector<cv::Point3d> points;
    triangulation(keypoints_1, keypoints_2, matches, R, t, points);

    // Calibrated Camera Matrix
    cv::Mat K = ( cv::Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
    for(int i=0; i<matches.size(); i++){
        // 2d point in first frame
        cv::Point2d pt1_cam = pixel2cam(keypoints_1[matches[i].queryIdx].pt, K);
        // 3d point in 3D space 
        cv::Point2d pt1_cam_3d(
                points[i].x/points[i].z,
                points[i].y/points[i].z
            );

        // printf
        std::cout << "Point in the first camrea frame: " << pt1_cam << std::endl;
        std::cout << "Point project from 3D " << pt1_cam_3d << ", d=" << points[i].z << std::endl;
        
        // 2d point in second frame -> 3D -> 2D project
        cv::Point2f pt2_cam = pixel2cam(keypoints_2[matches[i].trainIdx].pt, K);
        cv::Mat pt2_trans = R*( cv::Mat_<double>(3,1) << points[i].x, points[i].y, points[i].z ) + t;

        // printf
        pt2_trans /= pt2_trans.at<double>(2,0);
        std::cout << "point in the second camera frame " << pt2_cam << std::endl;
        std::cout << "point reprojected from second frame: " << pt2_trans.t() << std::endl;
    }
    return 0;
}

void find_feature_matches(const cv::Mat& img_1, const cv::Mat& img_2,
                            std::vector<cv::KeyPoint>& keypoints_1,
                            std::vector<cv::KeyPoint>& keypoints_2, 
                            std::vector<cv::DMatch>& matches){
    // use ORB Algorithm
    cv::Mat descriptors_1, descriptors_2;
    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::ORB::create();
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");


    // find detect feature
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    // descriptors_matching 
    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    // Matching 
    std::vector<cv::DMatch> match;
    matcher->match(descriptors_1, descriptors_2, match);

    // find Max or Min distance value
    double min_dist = 10000, max_dist = 0;
    for(int i=0; i<descriptors_1.rows; i++){
        double dist = match[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }

    // Print
    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    // remove too far feature 
    for(int i=0; i<descriptors_1.rows; i++){
        if(match[i].distance <= std::max(2*min_dist, 30.0)){
            matches.push_back(match[i]);
        }
    }
}

void pose_estimation_2d2d(
    const std::vector<cv::KeyPoint>& keypoints_1,
    const std::vector<cv::KeyPoint>& keypoints_2,
    const std::vector<cv::DMatch>& matches,
    cv::Mat& R, cv::Mat& t){
    
    // Calibration Matrix
    cv::Mat K = ( cv::Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
    // Points vector
    std::vector<cv::Point2f> points1;
    std::vector<cv::Point2f> points2;

    // divide queryIdx and trainIdx 
    for(int i=0; i<(int)matches.size(); i++){
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        points2.push_back(keypoints_1[matches[i].trainIdx].pt);
    }

    // Find Fundamental Matrix , use 8_points algorithm
    cv::Mat fundamental_matrix;
    fundamental_matrix = cv::findFundamentalMat(points1, points2, cv::FM_8POINT);
    std::cout << "fundamental_matrix is " << std::endl << fundamental_matrix << std::endl;

    // centeroid point
    cv::Point2d principal_point(325.1, 249.7);
    // focal_length (lens - image plane)
    int focal_length = 521;
    cv::Mat essential_matrix;
    essential_matrix = cv::findEssentialMat(points1, points2, focal_length, principal_point);
    std::cout << "essential_matrix = " << essential_matrix << std::endl;

    // homograpy matrix
    cv::Mat homography_matrix;
    homography_matrix = cv::findHomography(points1, points2, cv::RANSAC, 3);
    std::cout << "homography_matrix = " << homography_matrix << std::endl;

    // After SVD operation, extract R, t in essential Matrix 
    cv::recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);
    std::cout<<"R is "<<std::endl<<R<<std::endl;
    std::cout<<"t is "<<std::endl<<t<<std::endl;
}

void triangulation(const std::vector<cv::KeyPoint>& keypoint_1, 
                    const std::vector<cv::KeyPoint>& keypoint_2, 
                    const std::vector<cv::DMatch>& matches,
                    const cv::Mat& R, const cv::Mat& t,
                    std::vector<cv::Point3d>& points){
    
    cv::Mat T1 = (cv::Mat_<float> (3,4) <<
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0);

    cv::Mat T2 = (cv::Mat_<float> (3,4) <<
        R.at<double>(0,0), R.at<double>(0,1), R.at<double>(0,2), t.at<double>(0,0),
        R.at<double>(1,0), R.at<double>(1,1), R.at<double>(1,2), t.at<double>(1,0),
        R.at<double>(2,0), R.at<double>(2,1), R.at<double>(2,2), t.at<double>(2,0)
    );

    cv::Mat K = ( cv::Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
    std::vector<cv::Point2f> pts_1, pts_2;
    for ( cv::DMatch m:matches ){
        pts_1.push_back ( pixel2cam( keypoint_1[m.queryIdx].pt, K) );
        pts_2.push_back ( pixel2cam( keypoint_2[m.trainIdx].pt, K) );
    }         

    cv::Mat pts_4d;
    cv::triangulatePoints(T1, T2, pts_1, pts_2, pts_4d);
        
    for ( int i=0; i<pts_4d.cols; i++ ){
        cv::Mat x = pts_4d.col(i);
        x /= x.at<float>(3,0);
        cv::Point3d p (
            x.at<float>(0,0), 
            x.at<float>(1,0), 
            x.at<float>(2,0) 
        );
        points.push_back( p );
    }
}


cv::Point2f pixel2cam ( const cv::Point2d& p, const cv::Mat& K ){
    return cv::Point2f(
        ( p.x - K.at<double>(0,2) ) / K.at<double>(0,0), 
        ( p.y - K.at<double>(1,2) ) / K.at<double>(1,1) 
    );
}


