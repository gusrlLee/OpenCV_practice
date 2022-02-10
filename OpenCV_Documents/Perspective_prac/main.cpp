#include "opencv2/opencv.hpp"
#include<iostream>
#include<vector>
#include<queue>

#define MAX_IMAGE_SIZE_WIDTH 500
#define MAX_IMAGE_SIZE_HEIGHT 800

using namespace std;
using namespace cv;

int main(){
    Mat image = imread("../demo.png");
    Mat trans_matrix;
    Mat dst;
    resize(image, image, Size(720, 480));

    printf("image width: %d\n", image.cols);
    printf("image heigth: %d\n", image.rows);

    vector<Point2f> src_coordinate_list(4);
    src_coordinate_list[0] = Point2f(260, 240);
    src_coordinate_list[1] = Point2f(460, 240);
    src_coordinate_list[2] = Point2f(660, 450);
    src_coordinate_list[3] = Point2f(60, 450);

    for(int i=0; i<src_coordinate_list.size(); i++){
        circle(image, src_coordinate_list[i], 5, Scalar(0, 0, 255), 5);
    }

    vector<Point2f> dst_coordinate_list(4);
    dst_coordinate_list[0] = Point2f(0,0);
    dst_coordinate_list[1] = Point2f(MAX_IMAGE_SIZE_WIDTH, 0);
    dst_coordinate_list[2] = Point2f(MAX_IMAGE_SIZE_WIDTH, MAX_IMAGE_SIZE_HEIGHT);
    dst_coordinate_list[3] = Point2f(0, MAX_IMAGE_SIZE_HEIGHT);

    trans_matrix = getPerspectiveTransform(src_coordinate_list, dst_coordinate_list);

    for(int i=0; i<trans_matrix.rows; i++){
        for(int j=0; j<trans_matrix.cols; j++){
            printf("%d ", trans_matrix.at<int>(Point(j, i)));
        }
        printf("\n");
    }

    warpPerspective(image, dst, trans_matrix, Size(MAX_IMAGE_SIZE_WIDTH, MAX_IMAGE_SIZE_HEIGHT));

    imshow("hello world", image);
    imshow("dst", dst);
    waitKey();
    destroyAllWindows();
}