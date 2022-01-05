#include "opencv2/opencv.hpp"

#include<vector>
#include<iostream>

using namespace std;
using namespace cv;

int main(){
    Mat dst, cdst, cdstP;
    vector<Vec2f> lines;

    Mat image = imread("../building.jpg");
    if(image.empty()){
        cerr << "NOT OPEN IMAGE\n";
        exit(1);
    }
    resize(image, image, Size(720, 480));

    // cvtColor(image, dst, COLOR_BGR2GRAY);

    Canny(image, cdst, 120, 250, 3);

    HoughLines(cdst, lines, 1, CV_PI / 150, 150, 0, 0);
    for(int i=0; i<lines.size(); i++){
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;

        double a = cos(theta);
        double b = sin(theta);

        double x0 = a * rho;
        double y0 = b * rho;

        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));

        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        line(cdst, pt1, pt2, Scalar(255, 255, 255), 3, LINE_AA);
    }

    imshow("image", image);
    // imshow("dst", dst);
    imshow("cdst", cdst);
    waitKey();
    destroyAllWindows();

    return 0;
}