#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

const char* src_window_name = "src demo";
const char* coner_window_name = "harries coner";

void cornerHarris_demo( int, void* );

int main(int argc, char* argv[]){
    src = imread("../../data/building.jpg");

    if(src.empty()){
        printf("CANNOT OPEN IMAGE\n");
        return 1;
    }
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    namedWindow(src_window_name);
    createTrackbar("Threshold: ", src_window_name, &thresh, max_thresh, cornerHarris_demo);
    imshow(src_window_name, src);

    cornerHarris_demo(0, 0);
    waitKey(0);

    return 0;
}

void cornerHarris_demo(int , void*){
    int block_size = 2;
    int aperture_size = 3;
    double k = 0.04;

    Mat dst = Mat::ones(src.size(), CV_32FC1);
    cornerHarris(src_gray, dst, block_size, aperture_size, k);

    Mat dst_normal, dst_normal_scale;
    normalize(dst, dst_normal, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

    convertScaleAbs(dst_normal, dst_normal_scale);

    for(int i=0; i<dst_normal.rows; i++){
        for(int j=0; j<dst_normal.cols; j++){
            if((int)dst_normal.at<float>(i, j) > thresh){
                circle(dst_normal_scale, Point(j, i), 5, Scalar(0), 2, 8, 0);
            }
        }
    }
    namedWindow(coner_window_name);
    imshow(coner_window_name, dst_normal_scale);
}