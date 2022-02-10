#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(){
    Mat image1 = imread("../../data/box.png", IMREAD_GRAYSCALE);
    Mat image2 = imread("../../data/box_in_scene.png", IMREAD_GRAYSCALE);

    if(image1.empty() || image2.empty()){
        std::cout << "CANNOT OPEN IMAGES!\n";
        return -1;
    }

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int min_hessian = 400;
    Ptr<SURF> detector = SURF::create(min_hessian);

    // min_hessian == threshold 특징 추출 경계값
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(image1, noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(image2, noArray(), keypoints2, descriptors2);

    //-- Step2: Matching descriptor vectors with a brute force matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    std::vector< DMatch > matches;
    matcher->match(descriptors1, descriptors2, matches);

    //--Step3: Draw matches
    Mat image_matches;
    drawMatches(image1, keypoints1, image2, keypoints2, matches, image_matches);

    //--Show detected matches
    imshow("matches", image_matches);

    waitKey();
    return 0;
}