#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat src_gray, src;
int thresh = 100;
RNG rng(12345);

void thresh_callback(int, void*);

int main(){
    src = imread("../../data/lena.png");

    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3,3));

    const char* window_name = "Source";
    namedWindow(window_name, WINDOW_AUTOSIZE);

    imshow(window_name, src);

    const int max_thresh = 255;
    createTrackbar("Canny thresh", window_name, &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);

    waitKey();
    return 0;
}

void thresh_callback(int, void*){
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh*2);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

    for(size_t i=0; i<contours.size(); i++){
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256));
        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
    }
    imshow("result", drawing);
}