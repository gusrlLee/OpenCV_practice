#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat src, dst;
    Mat kernel;

    Point anchor = Point(-1, -1);
    double delta = 0;
    int ddept = -1;
    int kernel_size;
    int ind = 0;

    const char* window_name = "Fiter2D Demo";

    src = imread("../lena.png");

    if(src.empty()){
        printf("CANNOT OPEN IMAGE\n");
        return 1;
    }

    for(;;){
        // Update kernel size for a normalized box filter
        kernel_size = 3 + 2*(ind%5);
        kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);

        // Apply filter
        filter2D(src, dst, ddept, kernel, anchor, delta, BORDER_DEFAULT);
        imshow(window_name, dst);

        char c = (char)waitKey(500);
        if(c == 27){
            break;
        }
        ind++;
    }

    return 0;
}