#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool use_mask;
Mat image;
Mat templ;
Mat mask;
Mat result;

const char* image_window = "Source Image";
const char* result_window = "Result Window";

int match_method;
int max_trackbar = 5;

void matchingMethod(int, void*);

int main(){
    image = imread("../../data/src.jpg");
    templ = imread("../../data/match.jpg");

    if(image.empty() || templ.empty()){
        cout << "CANT NOT READ";
        return -1;
    }

    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);

    const char* trackBar_label = 
    "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";

    createTrackbar(trackBar_label, image_window, &match_method, max_trackbar, matchingMethod);

    matchingMethod(0, 0);
    waitKey(0);
    return 0;
}

void matchingMethod(int, void*){
    Mat image_display;
    image.copyTo(image_display);

    int result_cols = image.cols - templ.cols + 1;
    int result_rows = image.rows - templ.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);

    bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);

    if(use_mask && method_accepts_mask){
        matchTemplate(image, templ, result, match_method, mask);
    }
    else{
        matchTemplate(image, templ, result, match_method);
    }

    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    if(match_method == TM_CCOEFF || match_method == TM_SQDIFF_NORMED){
        matchLoc = minLoc;
    }
    else {
        matchLoc = maxLoc;
    }

    rectangle(image_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

    imshow(image_window, image_display);
    imshow(result_window, result);

    return ;
}