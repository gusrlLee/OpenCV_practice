#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap("../../data/worker-zone-detection.mp4");
    if(!cap.isOpened()){
        cout << "CANNOT OPENED VIDEO!\n";
        return -1;
    }

    Mat frame, frame_gray;
    Mat frame_foreground, frame_foreground_mask, frame_background;
    Mat result_frame;

    Mat element = getStructuringElement(0, Size(5, 5),Point(0, 0));

    cap >> frame_background;
    if(frame_background.empty()){
        cout << "CANNOT OPENED VIDEO!\n";
        return -1;
    }
    cvtColor(frame_background, frame_background, COLOR_BGR2GRAY);

    while(true){
        cap >> frame;
        if(frame.empty()){
            break;
        }
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        absdiff(frame_gray, frame_background, frame_foreground_mask);
        threshold(frame_foreground_mask, frame_foreground_mask, 50, 200, THRESH_BINARY);
        morphologyEx(frame_foreground_mask, frame_foreground_mask, 2, element);
        frame_foreground_mask.copyTo(result_frame);
        frame_gray.copyTo(result_frame, frame_foreground_mask);

        imshow("origin", frame);
        imshow("Mask", frame_foreground_mask);
        imshow("result", result_frame);

        int key = waitKey(30);
        if(key == 27){
            break;
        }        
    }

    return 0;

}