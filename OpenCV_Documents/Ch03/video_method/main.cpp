#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void video_in();
void camera_in();

int main(){
    video_in();
    return 0;
}

void camera_in(){
    VideoCapture cap(0);

    if(!cap.isOpened()){
        cerr << "CANNOT OPEN CAMERA!!\n";
        return;
    }

    cout << "Frame width : " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << "\n";
    cout << "Frame height : " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << "\n";

    Mat frame, inversed;

    while(1){
        cap >> frame;
        if(frame.empty())
            break;

        inversed = ~frame;
        imshow("frame", frame);
        imshow("inversed", inversed);

        if(waitKey(10) == 27)
            break;
    }
    destroyAllWindows();
}

void video_in(){
    VideoCapture cap("../cut01.mp4");

    if(!cap.isOpened()){
        cerr << "NOT OPEN VIDEO!!";
        return;
    }

    cout << "Frame width" << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << "\n";
    cout << "Frame height" << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << "\n";
    cout << "Frame count" << cvRound(cap.get(CAP_PROP_FRAME_COUNT)) << "\n";

    double fps = cap.get(CAP_PROP_FPS);
    cout << "Frame Rate" << cvRound(cap.get(CAP_PROP_FPS)) << "\n";

    int delay = cvRound(1000 / fps);

    Mat frame, inversed;

    while(true){
        cap >> frame;

        if(frame.empty())
            break;
        
        inversed = ~frame;

        imshow("frame", frame);
        imshow("inverse", inversed);

        if(waitKey(5) == 27) break;
    }

    destroyAllWindows();
}