#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/features2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

int main(){
    Mat image1 = imread("../../data/box.png", IMREAD_GRAYSCALE);
    Mat image2 = imread("../../data/box_in_scene.png", IMREAD_GRAYSCALE);

    if(image1.empty() || image2.empty()){
        std::cout << "CANNOT OPEN IMAGE\n";
        return -1;
    }

    int min_hessian = 400;
    Ptr<SURF> detector = SURF::create(min_hessian);
    std::vector<KeyPoint> keypoint1, keypoint2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(image1, noArray(), keypoint1, descriptors1);
    detector->detectAndCompute(image2, noArray(), keypoint2, descriptors2);

    //--Step 2: Matching desriptor vectors with a FLANN based matcher
    // Sinece SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<DMatch>> knn_matches;
    matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);

    //--Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    std::vector<DMatch> good_matches;
    for(size_t i=0; i<knn_matches.size(); i++){
        if(knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance){
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    //-- Draw matches
    Mat image_matches;
    drawMatches(image1, keypoint1, image2,keypoint2, good_matches, image_matches, Scalar::all(-1), 
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    //-- Show detected matches
    imshow("Good Matches", image_matches);
    waitKey();
    return 0;
}