#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;

int low_threshold = 0;
const int max_low_threshold = 100;
const int r = 3; 
const int kernel_size = 3;
const char* window_name = "Edge Demo";

static void cannyThreshold(int a, void* b){
    blur(src_gray, detected_edges, Size(3, 3));

    Canny(detected_edges, detected_edges, low_threshold, low_threshold * r, kernel_size);
    dst = Scalar::all(0);
    src.copyTo(dst, detected_edges);
    imshow(window_name, dst);
}

int main(){
    src = imread("../../../data/lena.jpg");
    if(src.empty()){
        cout << "CANNOT OPENED IMAEG";
        return -1;
    }

    dst.create(src.size(), src.type());
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    
    namedWindow(window_name, WINDOW_AUTOSIZE);

    createTrackbar("Min Threshold", window_name, &low_threshold, max_low_threshold, cannyThreshold);

    cannyThreshold(0, 0);

    waitKey(0);
    return 0;
}

