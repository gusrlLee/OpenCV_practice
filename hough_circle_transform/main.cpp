#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>

using namespace std;
using namespace cv;

int main(){
    Mat src = imread("../../data/coin.jpg");
    if(src.empty()){
        cout << "CANNOT OPEN IMAGE\n";
        return -1;
    }

    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    imshow("gray", gray);
    Mat temp;
    temp = gray.clone();
    Canny(temp, temp, 150, 255, 3);
    imshow("Canny", temp);
    blur(temp, temp, Size(3,3));

    vector<Vec3f> circles;
    HoughCircles(
        gray, // input matrix (need to gray scale)
        circles, // output vector list
        HOUGH_GRADIENT, // method
        1, // dp : 입력 영상과 누적배열 비율
        50, // min_dist : 검출된 원 중심점들이 최소 거리
        150, // param_1 : Canny edge의 upper threshold
        10, // param_2 : 누적 배열에서 원 검출을 위한 threshold
        0, // min_radius : 원의 최소 반지름
        30 // max_radius : 원의 최대 반지름
    );

    for(size_t i=0; i<circles.size(); i++){
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(src, center, radius, Scalar(255, 0, 255), LINE_AA);
    }

    imshow("detected circle", src);
    waitKey(0);

    return 0;
}