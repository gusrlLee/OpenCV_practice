#include<iostream>
#include<opencv2/opencv.hpp>

int threshold_value = 0;
int threshold_type = 3;
const int max_value = 255;
const int max_type = 4;
int const max_binary_value = 255;

cv::Mat src, src_gray, dst;
const char* window_name = "Threshold Demo";

const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
const char* trackbar_value = "Value";

static void thresholdDemo(int, void*){
    /* 
    0: Binary
    1: Binary Inverted
    2: Threshold Truncated
    3: Threshold to Zero
    4: Threshold to Zero Inverted 
    */
    
    cv::threshold(src_gray, dst, threshold_value, max_binary_value, threshold_type);
    cv::imshow(window_name, dst);
}

int main(){
    src = cv::imread("../../data/lena.png");

    if(src.empty()){
        std::cout << "CANNOT OPENED IMAGE\n";
        return -1;
    }

    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar(trackbar_type, window_name, &threshold_type, max_type, thresholdDemo);
    cv::createTrackbar(trackbar_value, window_name, &threshold_value, max_value, thresholdDemo);

    thresholdDemo(0, 0);
    cv::waitKey(0);
    return 0;
}