// focal length 350 cx 240 cy 360
#include<iostream>
#include<opencv2/opencv.hpp>
#include<fstream>
#include<string>
#include<cmath>

// OpenCV
#include<opencv2/calib3d/calib3d.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/viz.hpp>
#include<opencv2/sfm.hpp>


void findFeatureMatches(const cv::Mat& image1, const cv::Mat& image2, 
                        std::vector<cv::KeyPoint>& keypoints_1, 
                        std::vector<cv::KeyPoint>& keypoints_2,
                        std::vector<cv::DMatch>& matches);

void triangulation(
    const std::vector<cv::KeyPoint>& keypoints_1, 
    const std::vector<cv::KeyPoint>& keypoints_2, 
    const std::vector<cv::DMatch>& matches,
    const cv::Mat& R, const cv::Mat& t, const cv::Mat& K,
    std::vector<cv::Point3d>& point_cloud
);

cv::Point2f pixel2cam(const cv::Point2d& p, const cv::Mat& K);

int main(int argc, char** argv){
    // define visualization
    cv::viz::Viz3d window("Coordinate Frame");
    window.showWidget("Coordinate Widget", cv::viz::WCoordinateSystem());
    window.setWindowSize(cv::Size(1500, 1500));
    window.setWindowPosition(cv::Point(150, 150));
    window.setBackgroundColor(); // black

    // define file stream object 
    std::ifstream data_list;
    // data file list
    data_list.open("../data_list.txt");
    std::string data_path;

    // data path string vector
    std::vector<std::string> data_path_list;

    // define image matrix and descriptor matrix
    cv::Mat curr_image, prev_image;

    // prev and curr R, t matrix
    cv::Mat prev_R, prev_t;

    // define instrnic matrix in camera calibration
    cv::Point2d principal_point(240, 360);
    int cx = 240; int cy = 360;
    int focal_length = 350;

    // for kitti dataset calibration
    //double focal_length = 718.8560;
    //cv::Point2d principal_point(607.1928, 185.2157);
    //double cx = 607.1928; double cy = 185.2157;

    // define intstrnic Matrix
    cv::Mat K = (cv::Mat_<double> (3,3) <<
            focal_length,   0,              cx,
            0,              focal_length,   cy,
            0,              0,              1);

    // data load 
    if(data_list.is_open()){
        while(data_list){
            getline(data_list, data_path);
            data_path_list.push_back(data_path);
            //std::cout << data_path << std::endl;
        }
        data_list.close();

    } else {
        printf("OPEN FAILED!!\n");
        exit(1);
    }

    std::cout << "data size = " << data_path_list.size() << "\n";

    int data_number = data_path_list.size();

    // point cloud
    std::vector<cv::Point3d> point_cloud;
    std::vector<cv::Vec3f> point_cloud_est;
    // camera Pose path
    std::vector<cv::Affine3d> camera_path;
        
    // first, define prev image load
    prev_image = cv::imread(data_path_list[0], 1);
    if(prev_image.empty()){
        exit(1);
    }

    // MAIN FUCNTION ================================================================
    for(int i=1; i<data_number-1; i++){
        printf("\n===============================================================\n");
        curr_image = cv::imread(data_path_list[i]);
        if(curr_image.empty())
            break;

        // make instence descriptor
        cv::Mat curr_descriptors, prev_descriptors;

        // feature coordinate list
        std::vector<cv::KeyPoint> curr_keypoints;
        std::vector<cv::KeyPoint> prev_keypoints;
        std::vector<cv::DMatch> match;

        // find feature and matching descriptor
        findFeatureMatches(prev_image,
                            curr_image, 
                            prev_keypoints, 
                            curr_keypoints,
                            match);

        // check Matching KeyPoints
        cv::Mat image_good_match;
        drawMatches(prev_image, prev_keypoints, curr_image, curr_keypoints, match, image_good_match);
        cv::imshow("good", image_good_match);
    
        // points1 = queryIdx
        // points2 = trainIdx
        std::vector<cv::Point2f> points1, points2;
        // seperation trainIdx and qureyIdx
        for(int t=0; t<(int)match.size(); t++){
            points1.push_back(prev_keypoints[match[t].queryIdx].pt);
            points2.push_back(curr_keypoints[match[t].trainIdx].pt);
        }

        // check vector size
        // std::cout << "points1 size = " << points1.size() << std::endl;  
        // std::cout << "points2 size = " << points2.size() << std::endl;  

        // find Essential Matrix
        if(points1.size() == 0 || points2.size() == 0) continue;
        cv::Mat essential_matrix, mask;
        essential_matrix = cv::findEssentialMat(points1, points2, focal_length, principal_point, cv::RANSAC, 0.999, 1, mask);

        cv::Mat R, t;

        // SVD operation and extract Rotation Matrix and translation Matrix
        std::cout << "Matrix = " << essential_matrix.isContinuous() << std::endl; 
        
        // error ouccur : essential matrix not continuous
        if(essential_matrix.isContinuous() == 0) continue;
        
        // operation SVD So, we extract Rotation Matrix and Translation Matrix from Essential Matrix
        cv::recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point, mask);

        std::cout << "Rotation Matrix = " << std::endl << R << std::endl;
        std::cout << "translation Matrix = " << std::endl << t << std::endl;

        // triangluration estimation 3D points
        triangulation(prev_keypoints, curr_keypoints, match, R, t, K, point_cloud);

        // check points cloud size 
        std::cout << "Points cloud size = " << point_cloud.size() << std::endl;

        if(i != 1){
            std::cout << "curr_R" << std::endl << prev_R << std::endl;
            std::cout << "curr_t" << std::endl << prev_t << std::endl;

            cv::Mat curr_R, curr_t;

            curr_R = R * prev_R;
            curr_t = prev_t + (curr_R * t);

            std::cout << "curr_R" << std::endl << curr_R << std::endl;
            std::cout << "curr_t" << std::endl << curr_t << std::endl;

            camera_path.push_back(cv::Affine3d(curr_R, curr_t));

            prev_R = curr_R.clone();
            prev_t = curr_t.clone();
        } else {
            prev_R = R.clone();
            prev_t = t.clone();
        }


        // add widget point cloud in 3D visualization
        cv::viz::WCloud cloud_widget(point_cloud, cv::viz::Color::green());
        window.showWidget("point_cloud", cloud_widget);

        // add widget Camera Pose
        if(camera_path.size() > 0){
            cv::viz::WTrajectory trajectory(camera_path, cv::viz::WTrajectory::FRAMES, 0.5);
            cv::viz::WTrajectoryFrustums frustums(camera_path, cv::Vec2f(0.889484, 0.523599), 0.5, cv::viz::Color::yellow());

            window.showWidget("camera", trajectory);
            window.showWidget("frustums", frustums);
        }
        window.spinOnce(1, true);

        prev_image = curr_image.clone();
        if (i == 24) break;
    }
    window.spin();
    return 0;
}

// [x, y] -> [u, v]
cv::Point2f pixel2cam(const cv::Point2d& p, const cv::Mat& K){
    return cv::Point2f(
         ( p.x - K.at<double>(0,2) ) / K.at<double>(0,0), 
         ( p.y - K.at<double>(1,2) ) / K.at<double>(1,1) 
        );
}

void findFeatureMatches(const cv::Mat& image1, const cv::Mat& image2, 
                        std::vector<cv::KeyPoint>& keypoints_1, 
                        std::vector<cv::KeyPoint>& keypoints_2,
                        std::vector<cv::DMatch>& matches){
    
    cv::Mat descriptors_1, descriptors_2;

    // feature detect algorithm
    int min_hessian = 400;
    cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(min_hessian);
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);

    detector->detectAndCompute(image1, cv::noArray(), keypoints_1, descriptors_1);
    detector->detectAndCompute(image2, cv::noArray(), keypoints_2, descriptors_2);

    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher->knnMatch(descriptors_1, descriptors_2, knn_matches, 2);
    
    //std::cout << knn_matches.size() << std::endl;
    const float ratio_thresh = 0.5f;

    // good feature
    for(size_t i=0; i<knn_matches.size(); i++){
        if(knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance){
            matches.push_back(knn_matches[i][0]);
        }
    }
}

void triangulation(
    const std::vector<cv::KeyPoint>& keypoints_1, 
    const std::vector<cv::KeyPoint>& keypoints_2, 
    const std::vector<cv::DMatch>& matches,
    const cv::Mat& R, const cv::Mat& t, const cv::Mat& K,
    std::vector<cv::Point3d>& point_cloud){
    
    cv::Mat T1 = (cv::Mat_<float> (3,4) << 
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0);


    // Make Projection Matrix [R | t]
    cv::Mat T2 = (cv::Mat_<double> (3,4) << 
            R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), t.at<double>(0, 0),
            R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2), t.at<double>(1, 0),
            R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2), t.at<double>(2, 0));

    std::vector<cv::Point2f> pts_1;
    std::vector<cv::Point2f> pts_2;

    // seperation Idx
    for(cv::DMatch m:matches){
        pts_1.push_back(pixel2cam(keypoints_1[m.queryIdx].pt, K));
        pts_2.push_back(pixel2cam(keypoints_2[m.trainIdx].pt, K));
    }

    // triangulation 3D points
    cv::Mat pts_4d;
    cv::triangulatePoints(T1, T2, pts_1, pts_2, pts_4d);

    // homogenous coordinate [wx, wy, wz, w]
    for ( int i=0; i<pts_4d.cols; i++){
        // extract one coordinate
        cv::Mat x = pts_4d.col(i);
        // [wx, wy, wz, w] -> [x, y, z, 1]
        x /= x.at<float>(3,0);

        // Nomarlization Coordinate
        cv::Point3d p (
                x.at<float>(0,0), 
                x.at<float>(1,0), 
                x.at<float>(2,0) 
                );
        // push
        point_cloud.push_back( p );
    }
}

