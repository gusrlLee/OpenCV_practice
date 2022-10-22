#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

// DMatch
void find_feature_matches(
    const Mat &img_1, const Mat &img_2,
    std::vector<KeyPoint> &keypoints_1,
    std::vector<KeyPoint> &keypoints_2,
    std::vector<DMatch> &matches
);

// After DMatch and R, T;
void pose_estimation_2d2d(
    std::vector<KeyPoint> keypoints_1,
    std::vector<KeyPoint> keypoints_2,
    std::vector<DMatch> matches,
    Mat &R, 
    Mat &t
);


Point2d pixel2cam(const Point2d &p, const Mat &K);

int main(int argc, char** argv){
    // read image
    Mat img_1 = imread("../../../data/1.png");
    Mat img_2 = imread("../../../data/2.png");
    assert(img_1.data && img_2.data && "Cannot load images");

    // vector define
    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;
    // find feature ORB
    find_feature_matches(img_1, img_2, keypoints_1, keypoints_2, matches);

    cout << "matches size is " << matches.size() << endl;

    // find Rotation matrix and translation matrix
    Mat R, t;
    pose_estimation_2d2d(keypoints_1, keypoints_2, matches, R, t);

    Mat t_x = (Mat_<double>(3, 3) << 0, -t.at<double>(2, 0), t.at<double>(1, 0),
      t.at<double>(2, 0), 0, -t.at<double>(0, 0),
      -t.at<double>(1, 0), t.at<double>(0, 0), 0);

    cout << "t^R = " << endl << t_x * R << endl;
    // camrea intrinsic matrix
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);
    for (DMatch m: matches) {
        Point2d pt1 = pixel2cam(keypoints_1[m.queryIdx].pt, K);
        Mat y1 = (Mat_<double>(3, 1) << pt1.x, pt1.y, 1);
        Point2d pt2 = pixel2cam(keypoints_2[m.trainIdx].pt, K);
        Mat y2 = (Mat_<double>(3, 1) << pt2.x, pt2.y, 1);
        Mat d = y2.t() * t_x * R * y1;
        cout << "epipolar constraint = " << d << endl;
    }
    return 0;
}


void find_feature_matches(const Mat &img_1, const Mat &img_2, std::vector<KeyPoint> &keypoints_1, 
                        std::vector<KeyPoint> &keypoints_2, std::vector<DMatch> &matches){

    // ORB algorithm
    Mat descriptors_1, descriptors_2;
    // create Instance 
    Ptr<FeatureDetector> detector = ORB::create();
    // create descriptor
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    // find feature
    detector->detect(img_1, keypoints_1);
    // find feature
    detector->detect(img_2, keypoints_2);   

    // compute feature descriptor each image
    detector->compute(img_1, keypoints_1, descriptors_1);
    detector->compute(img_2, keypoints_2, descriptors_2);
 
    // match feature between image1 and image2
    vector<DMatch> match;
    matcher->match(descriptors_1, descriptors_2, match);
    // define
    double min_dist = 10000, max_dist = 0;

    // find Max distance and Min distance  
    for(int i=0; i<descriptors_1.rows; i++){
        double dist = match[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- min dist : %f \n", min_dist);

    for(int i=0; i<descriptors_1.rows; i++){
        if(match[i].distance <= max(2 * min_dist, 30.0)){
            matches.push_back(match[i]);
        }
    }
}

Point2d pixel2cam(const Point2d &p, const Mat &K){
    return Point2d((p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
    (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1));
}

void pose_estimation_2d2d(std::vector<KeyPoint> keypoints_1, std::vector<KeyPoint> keypoints_2, 
                        std::vector<DMatch> matches, Mat &R, Mat &t){
    
    // Calibrated Camrea matrix define
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    // feature vector
    vector<Point2f> points1;
    vector<Point2f> points2;

    // QueryIdx : standard point trainIdx : matched with standard point
    // create Vector
    for(int i=0; i<(int)matches.size(); i++){
        points1.push_back(keypoints_1[matches[i].queryIdx].pt);
        points2.push_back(keypoints_2[matches[i].trainIdx].pt);
    }

    // find Fundametal_matrix
    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat(points1, points2, FM_8POINT);
    cout << "fundametal_matrix is " << endl << fundamental_matrix << endl;

    // centroid points
    Point2d principal_point(325.1, 249.7);
    double focal_length = 521;
    // find essential matrix use OpenCV method
    Mat essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);
    cout << "essential matrix is " << endl << essential_matrix << endl;

    // use OpenCV Library method findHomography matrix and use RANSAC
    Mat homography_matrix;
    homography_matrix = findHomography(points1, points2, RANSAC, 3);
    cout << "Homography_matrix is " << endl << homography_matrix << endl;

    // recoverPose -> compute SVD, so we get Rotation matrix and Translation matrix 
    recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);
    cout << "R is " << endl << R << endl;
    cout << "t is " << endl << t << endl;

}
