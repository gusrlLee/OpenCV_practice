#include<iostream>
#include "opencv2/opencv.hpp"
#include<vector>

using namespace std;
using namespace cv;

int main(){
    Mat image = imread("../lena.png", 0);
    resize(image, image, Size(720, 480));
    printf("image rol : %d\n image col : %d\nimage size %d\n", image.rows, image.cols, image.type());
    Mat mask(Size(image.cols, image.rows) , CV_8UC1, Scalar(0));
    Mat dst(Size(image.cols, image.rows) , CV_8UC1, Scalar(0));
    
    Point p_list[4] = {
        Point(280, 240),
        Point(440, 240),
        Point(540, 480),
        Point(180, 480)
    };
    const Point* ppt[1] = {p_list};
    int npt[] = {4};

    fillPoly(mask, ppt, npt, 1, Scalar(255), 8);
    imshow("image", image);
    imshow("mask", mask);
    bitwise_and(image, image, dst, mask);
    imshow("result", dst);
    waitKey();
    destroyAllWindows();

    return 0;
}