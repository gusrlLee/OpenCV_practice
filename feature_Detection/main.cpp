#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/features2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(){
    Mat src = imread("../../data/box.png");
    if(src.empty()){
        std::cout << "CANNOT OPEN ERROR!\n";
        return -1;
    }

    // --Step 1: Detect the keypoints using SURF Detector
    int min_hessian = 400;
    Ptr<SURF> detector = SURF::create(min_hessian);
    // SURF, KAZE, AKAZE, ORB exist
    std::vector<KeyPoint> keypoints;
    detector->detect(src, keypoints);

    // -- Draw keypoints
    Mat image_keypoints;
    drawKeypoints(src, keypoints, image_keypoints);

    //-- Show detected (drawn) keypoints
    imshow("SURF Keypoints", image_keypoints);

    waitKey();
    return 0;
}