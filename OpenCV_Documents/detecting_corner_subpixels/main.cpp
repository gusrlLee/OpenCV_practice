#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src, src_gray;

int max_corner = 10;
int max_trackbar = 25;

RNG rng(12345);
const char* source_window_name = "Image";

void goodFeaturesToTrackDemo(int, void*);

int main(){
    src = imread("../../data/apple.jpg");

    if(src.empty()){
        cout << "CANNOT OPEN IMAGE";
        return -1;
    }

    cvtColor(src, src_gray, COLOR_RGB2GRAY);

    namedWindow(source_window_name);
    createTrackbar("Max corner", source_window_name, &max_corner, max_trackbar, goodFeaturesToTrackDemo);

    imshow(source_window_name, src);
    goodFeaturesToTrackDemo(0, 0);

    waitKey();
    return 0;
}

void goodFeaturesToTrackDemo(int, void*){
    max_corner = MAX(max_corner, 1);
    vector<Point2f> corners;
    double quality_level = 0.01;
    double min_distance = 10;
    int block_size = 3, gradient_size = 3;
    bool is_use_harris_detector = false;
    double k = 0.04;

    Mat copy = src.clone();

    goodFeaturesToTrack(src_gray, 
                        corners,
                        max_corner,
                        quality_level,
                        min_distance,
                        Mat(),
                        block_size,
                        gradient_size,
                        is_use_harris_detector,
                        k);

    cout << "** Number of corners detected : " << corners.size() << endl;
    int radius = 4;
    for(size_t i = 0; i < corners.size(); i++){
        circle(copy, corners[i], radius, Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), FILLED);
    }

    namedWindow(source_window_name);
    imshow(source_window_name, copy);

    Size window_size = Size(5, 5);
    Size zero_zone = Size(-1, -1);
    TermCriteria criteria = TermCriteria( TermCriteria::EPS + TermCriteria::COUNT, 40, 0.001 );
    cornerSubPix( src_gray, corners, window_size, zero_zone, criteria );
    for( size_t i = 0; i < corners.size(); i++ )
    {
        cout << " -- Refined Corner [" << i << "]  (" << corners[i].x << "," << corners[i].y << ")" << endl;
    }
}