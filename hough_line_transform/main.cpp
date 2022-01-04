#include "opencv2/opencv.hpp"

#include<vector>
#include<iostream>

using namespace std;
using namespace cv;

int main(){
    Mat dst;
    vector<Vec2f> lines;

    Mat image = imread("../building.jpg");
    if(image.empty()){
        cerr << "NOT OPEN IMAGE\n";
        exit(1);
    }
    resize(image, image, Size(720, 480));
    cvtColor(image, image, COLOR_BGR2GRAY);

    Canny(image, dst, 100, 255);

    HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0);

    for(int i=0; i<lines.size(); i++){
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        printf("%d : %f %f \n ", i, rho, theta);

        double a = cos(theta);
        double b = sin(theta);

        double x0 = a * rho;
        double y0 = b * rho;

    }

    imshow("image", dst);
    waitKey();
    destroyAllWindows();

    return 0;
}