#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat src, src_gaussian, src_gray, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    const char* window_name = "Laplacian demo";

    src = imread("../lena.png");
    if(src.empty()){
        printf("CANNOT OPEN IMAGE\n");
        return 1;
    }
    imshow("orign demo", src);
    GaussianBlur(src, src_gaussian, Size(3, 3), 0, 0, BORDER_DEFAULT);
    imshow("gaussian demo", src_gaussian);
    cvtColor(src_gaussian, src_gray, COLOR_BGR2GRAY);

    Mat abs_dst;
    Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);

    convertScaleAbs(dst, abs_dst);
    imshow(window_name, abs_dst);

    char c = (char)waitKey(0);

    return 0;
}
