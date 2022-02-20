#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<chrono>

using namespace std;
using namespace cv;

int main(int argc, char** argv){

    // Data load
    Mat img_1 = imread("../../../data/1.png");
    Mat img_2 = imread("../../../data/2.png");

    // exception 
    assert(img_1.data != nullptr && img_2.data != nullptr);


    // create ORB detector
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    // detection
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    // time check
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "extract ORB cost = " << time_used.count() << " seconds. " << endl;

    // draw keypoint
    Mat outimage1;
    drawKeypoints(img_1, keypoints_1, outimage1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("ORB features", outimage1);
    
    // Match between image1 and image2
    vector<DMatch> matches;
    t1 = chrono::steady_clock::now();
    matcher->match(descriptors_1, descriptors_2, matches);
    t2 = chrono::steady_clock::now();
    time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    cout << "match ORB cost = " << time_used.count() << " seconds. " << endl;


    auto min_max = minmax_element(matches.begin(), matches.end(),
                            [](const DMatch &m1, const DMatch &m2) { return m1.distance < m2.distance; });
    double min_dist = min_max.first->distance;
    double max_dist = min_max.second->distance;


    std::vector<DMatch> good_matches;
    for (int i = 0; i < descriptors_1.rows; i++) {
        if (matches[i].distance <= max(2 * min_dist, 30.0)) {
        good_matches.push_back(matches[i]);
        }
    }

    Mat image_match;
    Mat image_good_match;

    drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, image_match);
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches, image_good_match);
    imshow("all match", image_match);
    imshow("good matches", image_good_match);
    waitKey(0);


    return 0;
}