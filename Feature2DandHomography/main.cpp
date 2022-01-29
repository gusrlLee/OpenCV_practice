#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/features2d.hpp>

int main(){
    cv::Mat image_object = cv::imread("../../data/box.png");
    cv::Mat image_scene = cv::imread("../../data/box_in_scene.png");

    if(image_object.empty() || image_scene.empty()){
        std::cout << "CANNOT OPEN IMAGE!\n";
        return -1;
    }

    //--Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int min_hessian = 400;
    cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(min_hessian);
    std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
    cv::Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute(image_object, cv::noArray(), keypoints_object, descriptors_object);
    detector->detectAndCompute(image_scene, cv::noArray(), keypoints_scene, descriptors_scene);

    //--Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point type descriptor NORM_L2 is used
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<cv::DMatch>> knn_match;
    matcher->knnMatch(descriptors_object, descriptors_scene, knn_match, 2);

    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.75f;
    std::vector<cv::DMatch> good_matches;
    for(size_t i = 0; i < knn_match.size(); i++){
        if(knn_match[i][0].distance < ratio_thresh * knn_match[i][1].distance){
            good_matches.push_back(knn_match[i][0]);
        }
    }

    //-- Draw matches
    cv::Mat image_matches;
    cv::drawMatches(image_object, keypoints_object, image_scene, keypoints_scene, good_matches, image_matches, 
        cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    //-- Localize the object
    std::vector<cv::Point2f> obj;
    std::vector<cv::Point2f> scene;

    for(size_t i=0; i<good_matches.size(); i++){
        //-- Get the keypoints from the good matches
        obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
    }

    cv::Mat H = cv::findHomography(obj, scene, cv::RANSAC);

    //--Get the corners from the image_1 (the object to be "detected")
    std::vector<cv::Point2f> obj_corners(4);
    obj_corners[0] = cv::Point2f(0, 0);
    obj_corners[1] = cv::Point2f((float)image_object.cols, 0);
    obj_corners[2] = cv::Point2f((float)image_object.cols, (float)image_object.rows);
    obj_corners[3] = cv::Point2f(0, (float)image_object.rows);
    std::vector<cv::Point2f> scene_corners(4);

    cv::perspectiveTransform(obj_corners, scene_corners, H);

    //-- Draw lines between the corners (the mapped object in the scene - image_2)
    line(image_matches, scene_corners[0] + cv::Point2f((float)image_object.cols, 0), 
        scene_corners[1] + cv::Point2f((float) image_object.cols, 0), cv::Scalar(0, 255, 0), 4);
    line(image_matches, scene_corners[1] + cv::Point2f((float)image_object.cols, 0), 
        scene_corners[2] + cv::Point2f((float) image_object.cols, 0), cv::Scalar(0, 255, 0), 4);
    line(image_matches, scene_corners[2] + cv::Point2f((float)image_object.cols, 0), 
        scene_corners[3] + cv::Point2f((float) image_object.cols, 0), cv::Scalar(0, 255, 0), 4);
    line(image_matches, scene_corners[3] + cv::Point2f((float)image_object.cols, 0), 
        scene_corners[0] + cv::Point2f((float) image_object.cols, 0), cv::Scalar(0, 255, 0), 4);


    //--Show detected matches
    cv::imshow("Good Matches & Object detection", image_matches);
    cv::waitKey();

    return 0;
}