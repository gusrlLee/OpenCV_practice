#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat calcHistGray(const Mat& image);
Mat getGrayHistImage(const Mat& hist);

int main(){
    Mat image = imread("../lena.png", IMREAD_GRAYSCALE);
    imshow("image", image);
    imshow("Histogram", getGrayHistImage(calcHistGray(image)));

    waitKey();
    destroyAllWindows();
    return 0;
}

Mat calcHistGray(const Mat& image){
    CV_Assert(image.type() == CV_8UC1);

    Mat hist;
    int channels[] = {0};
    int dims = 1;
    int hist_size[] = {256};
    float gray_level[] = {0, 256};
    const float* ranges[] = { gray_level }; 
    

    calcHist(&image, 1, channels, noArray(), hist, dims, hist_size, ranges);
    return hist;
}

Mat getGrayHistImage(const Mat& hist) {
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.size() == Size(1, 256));

    double histMax;
    Mat image_hist(100, 256, CV_8UC1, Scalar(255));

    minMaxLoc(hist, 0, &histMax);

    for(int i=0; i<256; i++){
        line(image_hist, Point(i, 0), Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), Scalar(0));
    }
    return image_hist;
}
