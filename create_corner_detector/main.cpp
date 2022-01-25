#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat src, src_gray;
Mat my_harris_dst, my_harris_copy, mc;
Mat my_shi_tomasi_dst, my_shi_tomasi_copy;

int my_shi_tomasi_quality_level = 50;
int my_harris_quality_level = 50;
int max_quality_level = 100;

double my_harris_min_value, my_harris_max_value;
double my_shi_tomasi_min_value, my_shi_tomasi_max_value;

RNG rng(12345);

const char* my_harris_window_name = "Harris corner detector";
const char* my_shi_tomasi_window_name = "Shi Tomasi corner detector";

void myShiTomasiFunction(int, void*);
void myHarrisFunction(int, void*);

int main(int argc, char**argv){
    src = imread("../../data/building.jpg");
    if(src.empty()){
        cout << "CANNOT OPEN IMAEG\n";
        return -1;
    }

    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    int block_size = 3, aperture_size = 3;

    cornerEigenValsAndVecs(src_gray, my_harris_dst, block_size, aperture_size);

    mc = Mat(src_gray.size(), CV_32FC1);
    for(int i=0; i<mc.rows; i++){
        for(int j=0; j<mc.cols; j++){
            float lambda_1 = my_harris_dst.at<Vec6f>(i, j)[0];
            float lambda_2 = my_harris_dst.at<Vec6f>(i, j)[1];
            mc.at<float>(i, j) = lambda_1 * lambda_2 - 0.04f*((lambda_1 + lambda_2) * (lambda_1 + lambda_2));
        }
    }

    minMaxLoc(mc, &my_harris_min_value, &my_harris_max_value);

    namedWindow(my_harris_window_name);
    createTrackbar("Quality Level", my_harris_window_name, &my_harris_quality_level, max_quality_level, myHarrisFunction);
    myHarrisFunction(0, 0);

    cornerMinEigenVal(src_gray, my_shi_tomasi_dst, block_size, aperture_size);

    minMaxLoc(my_shi_tomasi_dst, &my_shi_tomasi_min_value, &my_shi_tomasi_max_value);

    namedWindow(my_shi_tomasi_window_name);
    createTrackbar("Quality Level", my_shi_tomasi_window_name, &my_shi_tomasi_quality_level, max_quality_level, myShiTomasiFunction);
    myShiTomasiFunction(0, 0);

    waitKey();
    return 0;
}

void myShiTomasiFunction(int, void*){
    my_shi_tomasi_copy = src.clone();
    my_shi_tomasi_quality_level = MAX(my_shi_tomasi_quality_level, 1);

    for(int i=0; i<src_gray.rows; i++){
        for(int j=0; j<src_gray.cols; j++){
            if(mc.at<float>(i, j) > my_shi_tomasi_min_value + (my_shi_tomasi_max_value - my_shi_tomasi_min_value) * my_shi_tomasi_quality_level/max_quality_level){
                circle(my_shi_tomasi_copy, Point(j, i), 4, Scalar(rng.uniform(0, 256),rng.uniform(0, 256),rng.uniform(0, 256)), FILLED);
            }
        }
    }

    imshow(my_shi_tomasi_window_name, my_shi_tomasi_copy);
}

void myHarrisFunction(int, void*){
    my_harris_copy = src.clone();
    my_harris_quality_level= MAX(my_harris_quality_level, 1);
    for(int i=0; i<src_gray.rows; i++){
        for(int j=0; j<src_gray.cols; j++){
            if( mc.at<float>(i,j) > my_harris_min_value + (my_harris_max_value - my_harris_min_value) * my_harris_quality_level/max_quality_level){
                circle(my_harris_copy, Point(j, i), 4, Scalar(rng.uniform(0, 256),rng.uniform(0, 256),rng.uniform(0, 256)), FILLED);
            }
        }
    }
    imshow(my_harris_window_name, my_harris_copy);
}