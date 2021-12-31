#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap("../cut01.mp4");
    Mat frame;
    Mat dst;
    Mat sobel;
    Mat sobelX;
    Mat sobelY;

    if(!cap.isOpened()){
        cerr << "NOT OPENED VIDEO!\n";
        exit(1);
    }

    float fps = cap.get(CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);

    while(true){
        cap >> frame;
        if(frame.empty()){
            break;
        }
        cvtColor(frame, dst, COLOR_BGR2GRAY);
        Sobel(dst, sobelX, CV_8UC1, 1, 0);
        Sobel(dst, sobelY, CV_8UC1, 0, 1);
        sobel = abs(sobelX) + abs(sobelY);


        imshow("result", frame);
        imshow("dst", sobel);
        if(waitKey(delay) == 27){
            break;
        }
    }
    destroyAllWindows();
    return 0;
}