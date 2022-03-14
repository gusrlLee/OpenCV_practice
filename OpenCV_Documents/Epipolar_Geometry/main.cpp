#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/calib3d.hpp>

using namespace std;
using namespace cv::xfeatures2d;
using namespace cv;

int main(int argc, char** argv){
    // load image -> gray image
    cv::Mat right_image = cv::imread("../../../data/left.jpg", IMREAD_GRAYSCALE);
    cv::Mat left_image = cv::imread("../../../data/right.jpg", IMREAD_GRAYSCALE);

    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    cv::Mat descriptors_1, descriptors_2; 

    // SURF detector
    int min_hessian = 500;
    Ptr<SURF> detector = SURF::create(min_hessian);

    detector->detectAndCompute( left_image, noArray(), keypoints_1, descriptors_1 );
    detector->detectAndCompute( right_image, noArray(), keypoints_2, descriptors_2 );

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<DMatch>> knn_matches;
    matcher->knnMatch(descriptors_1, descriptors_2, knn_matches, 2);

//    // ORB detector
//    cv::Ptr<FeatureDetector> detector = ORB::create();
//    cv::Ptr<DescriptorExtractor> descriptor = ORB::create();
//
//    // detection feature and compute
//    detector->detectAndCompute(left_image, noArray(), keypoints_1, descriptors_1);
//    detector->detectAndCompute(right_image, noArray(), keypoints_2, descriptors_2);
//
//    // define parameters for ORB feature detector
//    const static auto index_params = new cv::flann::IndexParams();
//    index_params->setAlgorithm(cvflann::FLANN_INDEX_LSH);
//    index_params->setInt("table_number", 6);
//    index_params->setInt("key_size", 12);
//    index_params->setInt("multi_probe_level", 1);
//
//    const static auto search_params = new cv::flann::SearchParams();
//    search_params->setInt("checks", 20);
//
//    std::vector<std::vector<DMatch>> knn_matches;
//    const static auto flann = cv::FlannBasedMatcher(index_params, search_params);
//    flann.knnMatch(descriptors_2, descriptors_1, knn_matches, 2);
//
    cout << "knn match vector size = " << knn_matches.size() << std::endl;

    const float ratio_thresh = 0.7f;
    std::vector<DMatch> good_matches;
    std::vector<Point2f> points1;
    std::vector<Point2f> points2;

    for(size_t i=0; i<knn_matches.size(); i++){
        if(knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance){
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    for(int i=0; i<(int)good_matches.size(); i++){
        points1.push_back(keypoints_1[good_matches[i].queryIdx].pt);
        points2.push_back(keypoints_2[good_matches[i].trainIdx].pt);
    }

    Mat fundamental_matrix = findFundamentalMat(points1, points2, FM_8POINT);

    cout << "F matrix is " << std::endl << fundamental_matrix << std::endl;
    cout << "points1 size = " << points1.size() << std::endl;
    cout << "points2 size = " << points2.size() << std::endl;

    std::vector<Point3f> eplines_1;
    std::vector<Point3f> eplines_2;

    // compute Correspond Epliines
    computeCorrespondEpilines(points2, 2, fundamental_matrix, eplines_1);
    computeCorrespondEpilines(points1, 1, fundamental_matrix, eplines_2);
    
    cvtColor(left_image, left_image, COLOR_GRAY2BGR);
    cvtColor(right_image, right_image, COLOR_GRAY2BGR);
    
    int width = left_image.size().width;
    int height = left_image.size().height;

    for(int i=0; i<eplines_1.size(); i++){
        // random color
        int r = std::rand() % 256;
        int g = std::rand() % 256;
        int b = std::rand() % 256;

        int x0 = 0;
        // 0 : x,  1 : z, 2 : y 
        int y0 = int(-1 * eplines_1[i].z/eplines_1[i].y);

        int x1 = width;
        int y1 = int(-1 * (eplines_1[i].z + eplines_1[i].x * width) / eplines_1[i].y);

        line(left_image, Point(x0, y0), Point(x1, y1), Scalar(b, g, r), 1, LINE_AA);

        int p_x = (int)points1[i].x;
        int p_y = (int)points1[i].y;
        circle(left_image, Point(p_x, p_y), 3, Scalar(r, g, b), 3);
    }

    for(int i=0; i<eplines_2.size(); i++){
        // random color
        int r = std::rand() % 256;
        int g = std::rand() % 256;
        int b = std::rand() % 256;

        int x0 = 0;
        int y0 = int(-1 * eplines_2[i].z / eplines_2[i].y);

        int x1 = width;
        int y1 = int(-1 * (eplines_2[i].z + eplines_2[i].x * width) / eplines_2[i].y);

        line(right_image, Point(x0, y0), Point(x1, y1), Scalar(b, g, r), 1, LINE_AA);

        int p_x = (int)points2[i].x;
        int p_y = (int)points2[i].y;
        circle(right_image, Point(p_x, p_y), 3, Scalar(r, g, b), 3);
    }

    imshow("left_image", left_image);
    imshow("right_image", right_image);
    waitKey();
    return 0;
}

