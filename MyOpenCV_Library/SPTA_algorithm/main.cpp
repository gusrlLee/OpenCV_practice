#include<iostream>
#include "opencv2/opencv.hpp"

#define TRUE 1
#define FALSE 0

int main(int argc, char** argv){
    cv::Mat image;
    cv::Mat gray_image;
    cv::Mat blured_image, edge_image;
    cv::Mat spta_image;

    // image load
    image = cv::imread("../../../data/lena.jpg");

    // make Canny Edge image
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::blur(gray_image, blured_image, cv::Size(3, 3));
    cv::Canny(blured_image, edge_image, 100, 200, 3);

    // image show
    cv::imshow("edge_image", edge_image);

    // SPTA algorithm logic
    spta_image = (edge_image.clone()) / 255;
    cv::Mat result = spta_image.clone();
    cv::imshow("spta_image", spta_image);

    for(int row=1; row<spta_image.rows-2; row++){
        for(int col=1; col<spta_image.cols-2; col++){
            int p = (int)spta_image.at<uchar>(row, col);
            int n0 = (int)spta_image.at<uchar>(row, col+1);
            int n1 = (int)spta_image.at<uchar>(row+1, col+1);
            int n2 = (int)spta_image.at<uchar>(row+1, col);
            int n3 = (int)spta_image.at<uchar>(row+1, col-1);
            int n4 = (int)spta_image.at<uchar>(row, col-1);
            int n5 = (int)spta_image.at<uchar>(row-1, col-1);
            int n6 = (int)spta_image.at<uchar>(row-1, col);
            int n7 = (int)spta_image.at<uchar>(row-1, col+1);
 
            if((p == TRUE) && 
                    (!n0 && (n4 && (n5 || n6 || n2 || n3) && (n6 || !n7) && (n2 || !n1))) || 
                    (!n4 && (n0 && (n1 || n2 || n6 || n7) && (n2 || !n3) && (n6 || !n5))) ||
                    (!n2 && (n6 && (n7 || n0 || n4 || n5) && (n0 || !n1) && (n4 || !n3))) || 
                    (!n6 && (n2 && (n3 || n4 || n0 || n1) && (n4 || !n5) && (n0 || !n7)))){
                result.at<uchar>(row, col) = 0;
            }
        }
    }
    
    cv::imshow("result", result * 255);
    cv::waitKey(0);

    return 0;
}
