#include<iostream>
#include<sstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const char* params 
    = "{ help h         |           | Print usage }"
      "{ input          | cut01.mp4 | Path to a video or a sequence of image }"
      "{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";

int main(int argc, char* argv[]){
    CommandLineParser parser(argc, argv, params);
    parser.about("This program shows how to use background subtraction methods provided by "
        " OpenCV. You can process both videos and images.\n");

    if(parser.has("help")){
        // print help information
        parser.printMessage();
    }
    
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    if(parser.get<String>("algo") == "MOG2"){
        pBackSub = createBackgroundSubtractorMOG2();
    }
    else{
        pBackSub = createBackgroundSubtractorKNN();
    }

    VideoCapture capture("../../../data/cars.mp4");
    if(!capture.isOpened()){
        cout << "CANNOT OPENED VIDEO!\n";
        return -1;
    }

    Mat frame, fgMask;
    while(true){
        capture >> frame;
        if(frame.empty()){
            break;
        }

        // update the background model
        pBackSub->apply(frame, fgMask);

        // get the frame number and write it on current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100, 20), cv::Scalar(255, 255, 255), -1);

        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(0, 0 ,0));

        // show the current frame and the fg mask
        imshow("frame", frame);
        imshow("fgmask", fgMask);

        // get the input from the keyboard
        int key = waitKey(30);
        if(key == 27){
            break;
        }
    }

    return 0;
}
