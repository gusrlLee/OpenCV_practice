#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<queue>

cv::Mat image;
cv::Mat gray;
cv::Mat label_image;

void myFloodFill(cv::Mat image, int x, int y, int label);

int main(int argc, char** argv){
    if(argc < 2){
        std::cout << "USAGE: command [YOUR_IMAGE]\n" << std::endl;
        return 1;
    }
    char* data_path = argv[1];
    std::cout << "Data image Path: " << data_path << std::endl; 
    // read image
    image = cv::imread(data_path);
    if(image.empty()){
        std::cerr << "ERROR cannot open image, Try Again!\n";
        return 1;
    }

    // make binary image for practice flood fill algorithm
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, gray, 180, 255, cv::THRESH_BINARY_INV);
    // show image
    cv::imshow("gray image", gray);

    int label = 1;
    // label_image = cv::Mat::zeros(gray.rows, gray.cols, CV_8UC1);
    label_image = gray.clone();
    // start for loop
    for(int y=1; y<gray.rows-1; y++){
        for(int x=1; x<gray.cols-1; x++){
            myFloodFill(gray, x, y, label);
            label++;
        }
    }

    cv::imshow("result image", label_image);
    cv::waitKey(0);
    return 0;
}

void myFloodFill(cv::Mat src, int x, int y, int label){
    int i=0, j=0, left=0, right=0;
    std::queue<std::pair<int, int>> buffer;
    std::pair<int, int> point = std::make_pair(y, x);
    buffer.push(point);
    while(!buffer.empty()){
        std::pair<int, int> coordinate = buffer.front();
        // std::cout << "coordinate : {" << coordinate.first << ", " << coordinate.second << "}\n" << std::endl;
        buffer.pop();
        j = coordinate.first;
        i = coordinate.second;
        if(j > 0 && j < src.rows-2 && i > 0 && i < src.cols-2){
            if((int)label_image.at<uchar>(j, i) == 0){
                left = i;
                right = i;
                while((int)label_image.at<uchar>(j, left-1) == 0) left--;
                while((int)label_image.at<uchar>(j, right+1) == 0) right++;
                for(int t=left; t<=right; t++){
                    label_image.at<uchar>(j, t) = (uchar)label;
                    if(label_image.at<uchar>(j-1, t) == 0 && 
                        (t == left || label_image.at<uchar>(j-1, t-1) != 0)){
                            std::pair<int, int> new_point = std::make_pair(j-1, t);
                            buffer.push(new_point);
                    }

                    if(label_image.at<uchar>(j+1, t) == 0 && 
                        (t == left || label_image.at<uchar>(j+1, t-1) != 0)){
                            std::pair<int, int> new_point = std::make_pair(j+1, t);
                            buffer.push(new_point);
                    }
                }
            }
        }
    }
    return;
}