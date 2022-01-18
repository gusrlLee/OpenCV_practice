#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat hue;
int bins = 25;

void Hist_and_Backproj(int, void*);

int main(){
    Mat src = imread("../../data/lena.png");

    if(src.empty()){
        cout << "CANNOT OPENED IMAGE\n";
        return -1;
    }

    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    hue.create(hsv.size(), hsv.depth());
    int ch[] = {0, 0};
    mixChannels(&hsv, 1, &hue, 1, ch, 1);
    /* 
        METHOD : mixChannels parameters
        &hsv : The source array from which the channels will be copied
        1 : The number of source arrays
        &hue : The destination array of the copied channels
        ch[] = {0, 0} : The array of index pairs indicating how the channels are copied
        1 : Number of index pairs 
    */

    const char* window_name = "Source image";
    namedWindow(window_name);
    createTrackbar("* Hue bins: ", window_name, &bins, 180, Hist_and_Backproj);
    Hist_and_Backproj(0, 0);

    imshow(window_name, src);
    waitKey(0);
    return 0;
}

void Hist_and_Backproj(int, void*){
    int hist_size = MAX(bins, 2);
    float hue_range[] = {150, 240};
    const float* ranges[] = {hue_range};

    Mat hist;
    calcHist(&hue, 1, 0, Mat(), hist, 1, &hist_size, ranges, true, false);
    normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

    Mat backproj;
    calcBackProject(&hue, 1, 0, hist, backproj, ranges, 1, true);

    imshow("Back projection", backproj);

    int w = 400, h = 400;
    int bin_w = cvRound((double) w / hist_size);
    Mat hist_image = Mat::zeros(h, w, CV_8UC3);

    for(int i=0; i<bins; i++){
        rectangle(hist_image, Point(i*bin_w, h), Point((i+1)*bin_w, h - cvRound(hist.at<float>(i)*255.0)), Scalar(0, 0, 255), FILLED);
    }
    imshow("Histogram", hist_image);
}