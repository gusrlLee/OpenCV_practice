#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<queue>

int main(int argc, char** argv){
    cv::Mat image;
    cv::Mat label_image;
    if(argc < 2){
        std::cout << "USAGE: command [YOUR_IMAGE]\n" << std::endl;
        return 1;
    }
    char* data_path = argv[1];
    std::cout << "Data image Path: " << data_path << std::endl; 
    // read image
    image = cv::imread(data_path);
    if(image.empty()){
        std::cout << "CANNOT image open! Try Again!\n";
        return -1;
    }

    label_image = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);

    for(int j=0; j<image.rows; j++){
        for(int i=0; i<image.cols; i++){
            if(i+50 < image.cols && j+50 < image.rows)
                label_image.at<cv::Vec3b>(j+50, i+50) = image.at<cv::Vec3b>(j, i);
        }
    }

    cv::imshow("origin image", image);
    cv::imshow("result image", label_image);
    cv::waitKey();
    return 0;
}