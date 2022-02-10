#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>

int main(int argc, char** argv){
    if(argc < 2){
        std::cout << "USAGE: command [YOUR_IMAGE]\n" << std::endl;
        return 1;
    }

    int value[256] = {0};
    char* data_path = argv[1];
    std::cout << "image Path : " << data_path << std::endl;

    cv::Mat histogram(256, 256, CV_8UC1, cv::Scalar(0));
    cv::Mat image;

    cv::Mat src = cv::imread(data_path);
    if(src.empty()){
        std::cout << "CANNOT Open image, Try Again!\n";
        return 1;
    }

    // convert BGR image -> GRAY image and resize 512x512 -> 100x100
    cv::cvtColor(src, image, cv::COLOR_BGR2GRAY);
    cv::resize(image, image, cv::Size(100, 100));

    // Image size 
    int w = image.rows;
    int h = image.cols;
    std::cout << "image size : " << image.size() << "\n";

    // calculate gray image pixel counter
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            value[(int)image.at<uchar>(i, j)]++;
        }
    }

    for(int i=0; i<256; i++){
        line(histogram, cv::Point(i, 256), cv::Point(i, 256 - value[i]), cv::Scalar(255));
    }
    
    cv::imshow("gary image", image);
    cv::imshow("histogram", histogram);
    cv::waitKey(0);
    return 0;
}