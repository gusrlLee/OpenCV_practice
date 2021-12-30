#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat calcGrayHist(const Mat& image);
Mat getGrayHistImage(const Mat& hist);

void histogram_stretching();
void histogram_equalization();

int main(){
    histogram_stretching();
    // histogram_equalization();

    return 0;
}

Mat calcGrayHist(const Mat& image){
    CV_Assert(image.type() == CV_8UC1);
    // CV_Assert 조건식이 TRUE면 함수를 진행, FALSE라면 error 발생시킨다.
    
    Mat hist;
    int channel[] = { 0 };
    int dims = 1;
    const int hist_size[] = { 256 };
    float graylevel[] = {0 , 256};
    const float* ranges[] = { graylevel };

    calcHist(&image, 1, channel, noArray(), hist, dims, hist_size, ranges);

    return hist;
}

Mat getGrayHistImage(const Mat& hist){
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.size() == Size(1, 256));

    double hist_max;
    minMaxLoc(hist, 0, &hist_max);

    Mat image_hist(100, 256, CV_8UC1, Scalar(255));
    for(int i=0; i<256; i++){
        line(image_hist, Point(i, 100), Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / hist_max)),Scalar(0));
    }
    return image_hist;
}

void histogram_stretching(){
    Mat image = imread("../lena.png", IMREAD_GRAYSCALE);

    if(image.empty()){
        cerr << "CANNOT OPEN IMAGE\n";
        return;
    }

    double gmin, gmax;
    minMaxLoc(image, &gmin, &gmax);

    Mat dst = (image - gmin) * 255 / (gmax - gmin);
    imshow("image", image);
    imshow("image Histogram", getGrayHistImage(calcGrayHist(image)));

    imshow("image", dst);
    imshow("image Histogram", getGrayHistImage(calcGrayHist(dst)));

    waitKey();
    destroyAllWindows();
}

void histogram_equalization(){
    Mat image = imread("../lena.png", IMREAD_GRAYSCALE);

    if(image.empty()){
        cerr << "CANNOT OPEN IMAGE\n";
        return;
    }

    Mat dst;
    equalizeHist(image, dst);

    imshow("image", image);
    imshow("image Histogram", getGrayHistImage(calcGrayHist(image)));

    imshow("image", dst);
    imshow("image Histogram", getGrayHistImage(calcGrayHist(dst)));

    waitKey();
    destroyAllWindows();

}




