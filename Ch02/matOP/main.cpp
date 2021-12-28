#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void matOp1();

int main(){
    matOp1();
}

void matOp1(){
    Mat image1;

    Mat image2(480, 640, CV_8UC1);
    Mat image3(480, 640, CV_8UC3);

    Mat image4(Size(640, 480), CV_8UC1);

    Mat image5(480, 640, CV_8UC1, Scalar(128));
    Mat image6(480, 640, CV_8UC3, Scalar(0, 0, 255));

    Mat mat1 = Mat::zeros(3, 3, CV_32SC1);
    Mat mat2 = Mat::ones(3, 3, CV_32SC1);
    Mat mat3 = Mat::eye(3, 3, CV_32SC1);

    float data[] = {1, 2, 3, 4, 5, 6};
    Mat mat4(2, 3, CV_32FC1, data);

    mat4.create(256, 256, CV_8UC3);

    mat4 = Scalar(255, 0, 0);

    cout << "Mat 4 = " << mat4 << "\n";
}