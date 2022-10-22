#include<iostream>
#include<unistd.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

int main(){
    cout << "Hello OpenCV" << CV_VERSION << "\n";
    //cout << getcwd() << "\n";   
    Mat image = imread("lena.png");
    cout << getBuildInformation() << endl;
    if(image.empty()){
        cerr << "Image load failed\n";
        return -1;
    }

    namedWindow("image");
    imshow("image", image);

    waitKey(0);
    destroyAllWindows();
    return 0;
}
