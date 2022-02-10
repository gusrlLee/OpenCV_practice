#include<iostream>
#include<cmath>
#include<opencv2/opencv.hpp>

#define MAX_PIXEL_VALUE 255
#define MIN_PIXEL_VALUE 0


int main(int argc, char** argv){
    if(argc < 2){
        std::cout << "USAGE: command [YOUR_IMAGE]\n" << std::endl;
        return 1;
    }
    char* data_path = argv[1];
    std::cout << "Image Path : " << data_path << "\n";
    
    int pixel_values[256] = {0};
    float pixel_values_vector[256] = {0.0};
    
    cv::Mat gray_image;

    cv::Mat src = cv::imread(data_path);
    if(src.empty()){
        std::cerr << "ERROR! CANNOT open image, Try Again!\n" << std::endl;
        return 1;
    }

    // image transform gray image and resize image shape 300x300
    cv::cvtColor(src, gray_image, cv::COLOR_BGR2GRAY);
    cv::resize(gray_image, gray_image, cv::Size(300, 300));
    cv::imshow("gray image", gray_image);

    // get pixel value in image and make array for histogram
    int width = gray_image.rows;
    int height = gray_image.cols;

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            pixel_values[(int)gray_image.at<uchar>(i,j)]++;
        }
    }

    for(int i=0; i<=MAX_PIXEL_VALUE; i++){
        pixel_values_vector[i] = (float)pixel_values[i] / (float)(width * height);
    }

    // weights
    float w1[256] = {0.0};
    float w2[256] = {0.0};
    
    float w1_temp = 0.0;
    float w2_temp = 0.0;

    // Dispersion
    float d1[256] = {0.0};
    float d2[256] = {0.0};

    float d1_temp = 0.0;
    float d2_temp = 0.0;

    float m1[256] = {0.0};
    float m2[256] = {0.0};
    
    float m1_temp = 0.0;
    float m2_temp = 0.0;

    // sum weights
    for(int t=0; t<MAX_PIXEL_VALUE; t++){
        for(int i=0; i<=t; i++){
            w1_temp += pixel_values_vector[i];
        }
        for(int j=t+1; j<=MAX_PIXEL_VALUE; j++){
            w2_temp += pixel_values_vector[j];
        }
        w1[t] = w1_temp;
        w2[t] = w2_temp;
        w1_temp = 0.0; 
        w2_temp = 0.0;
    }


    for(int t=0; t<=MAX_PIXEL_VALUE; t++){
        for(int i=0; i<=t; i++){
            m1_temp += ((float)i * pixel_values_vector[i]);
        }
        for(int j=t+1; j<=MAX_PIXEL_VALUE; j++){
            m2_temp += ((float)j * pixel_values_vector[j]);
        }
        if(w1[t] != 0)
            m1[t] = (m1_temp / w1[t]);
        if(w2[t] != 0)
            m2[t] = (m2_temp / w2[t]);
        m1_temp = 0.0; 
        m2_temp = 0.0;
    }

    for(int t=0; t<=MAX_PIXEL_VALUE; t++){
        for(int i=0; i<=t; i++){
            d1_temp += (pixel_values_vector[i] * pow((float)i - m1[i], 2));
        }
        for(int j=t+1; j<=MAX_PIXEL_VALUE; j++){
            d2_temp += (pixel_values_vector[j] * pow((float)j - m2[j], 2));
        }
        if(w1[t] != 0)
            d1[t] = (d1_temp / w1[t]);
        if(w2[t] != 0)
            d2[t] = (d2_temp / w2[t]);
        d1_temp = 0.0; 
        d2_temp = 0.0;
    }

    // threshold value
    long int optim_t = 9999999;
    long int value = 0;
    long int value_index = 0;

    for(int t=0; t<=MAX_PIXEL_VALUE; t++){
        int a = w1[t] * d1[t];
        int b = w2[t] * d2[t];
        std::cout << a << "\n" << b << "\n";
        value = a + b;
        if(value < optim_t && value != 0){
            optim_t = value;
            value_index = t;
        }   
        
    }

    int threshold = value_index;
    std::cout << "Optimizer threshold : " << threshold << "\n";
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            if((int)gray_image.at<uchar>(x, y) > threshold){
                gray_image.at<uchar>(x,y) = (uchar)255;
            }
            else{
                gray_image.at<uchar>(x,y) = (uchar)0;
            }
        }
    }
    
    cv::imshow("binary image", gray_image);
    cv::waitKey(0);
    return 0;
}