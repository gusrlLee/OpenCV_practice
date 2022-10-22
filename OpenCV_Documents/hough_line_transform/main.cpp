#include "opencv2/opencv.hpp"

#include<vector>
#include<iostream>

using namespace std;
using namespace cv;

int main(){
    Mat dst, cdst, cdstP;
    vector<Vec2f> lines;

    Mat image = imread("../../../data/dot1.png");
    if(image.empty()){
        cerr << "NOT OPEN IMAGE\n";
        exit(1);
    }
    resize(image, image, Size(512, 512));

    cvtColor(image, dst, COLOR_BGR2GRAY);

    Canny(dst, cdst, 150, 250, 3);

    HoughLines(cdst, lines, 1, CV_PI / 150, 8, 0, 0);
    for(int i=0; i<lines.size(); i++){
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;

        double a = cos(theta);
        double b = sin(theta);

        double x0 = a * rho;
        double y0 = b * rho;

        pt1.x = cvRound(x0 + 250*(-b));
        pt1.y = cvRound(y0 + 250*(a));

        pt2.x = cvRound(x0 - 250*(-b));
        pt2.y = cvRound(y0 - 250*(a));
        std::cout << pt1 << pt2 << std::endl;

        line(cdst, pt1, pt2, Scalar(255, 255, 255), 3, LINE_AA);
    }

    imshow("image", image);
    // imshow("dst", dst);
    imshow("cdst", cdst);
    waitKey();
    destroyAllWindows();

    return 0;
}
