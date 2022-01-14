#include<iostream>
#include<opencv2/opencv.hpp>

cv::Mat image, erosion_dst, dilation_dst; 
int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

void erosion(int, void*);
void dilation(int, void*);

int main(){
    image = cv::imread("../image.png");
    if(image.empty()){
        std::cout << "Could not open or find the image\n" << std::endl;
        return -1;
    }

    cv::namedWindow("origin", cv::WINDOW_AUTOSIZE);
    cv::imshow("origin", image);
    cv::namedWindow("Erosion Demo", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Dilation Demo", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Dilation Demo", image.cols, 0);

    cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo", &erosion_elem, max_elem, erosion);
    cv::createTrackbar("Kernel size: \n 2n + 1", "Erosion Demo", &erosion_size, max_kernel_size, erosion);
    cv::createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo", &dilation_elem, max_elem, dilation);
    cv::createTrackbar("Kernel size: \n 2n + 1", "Dilation Demo", &dilation_size, max_kernel_size, dilation);


    erosion(0, 0);
    dilation(0, 0);

    cv::waitKey(0);
    return 0;
}

void erosion(int, void*){
    int erosion_type = 0;
    if(erosion_elem == 0){
        erosion_type = cv::MORPH_RECT;
    }
    else if(erosion_elem == 1){
        erosion_type = cv::MARKER_CROSS;
    }
    else if(erosion_elem == 2){
        erosion_type = cv::MORPH_ELLIPSE;
    }

    cv::Mat element = cv::getStructuringElement(erosion_type, cv::Size(2*erosion_size+1, 2*erosion_size+1), cv::Point(erosion_size, erosion_size));
    cv::erode(image, erosion_dst, element);
    cv::imshow("Erosion Demo", erosion_dst);
}

void dilation(int, void*){
    int dilation_type = 0;
    if(dilation_elem == 0){
        dilation_type = cv::MORPH_RECT;
    }
    else if(dilation_elem == 1){
        dilation_type = cv::MARKER_CROSS;ma
    }
    else if(dilation_elem == 2){
        dilation_type = cv::MORPH_ELLIPSE;
    }

    cv::Mat element = cv::getStructuringElement(dilation_type, cv::Size(2*dilation_size+1, 2*dilation_size+1), cv::Point(dilation_size, dilation_size));
    cv::dilate(image, dilation_dst, element);
    cv::imshow("Dilation Demo", dilation_dst);
}