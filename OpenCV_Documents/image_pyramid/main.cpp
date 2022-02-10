#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(){
    Mat src;
    src = imread("../../data/lena.png");

    cout << "\n Zoom In-Out demo \n "
        "------------------  \n"
        " * [i] -> Zoom in   \n"
        " * [o] -> Zoom out  \n"
        " * [ESC] -> Close program \n" << endl;

    if(src.empty()){
        cout << "CANNOT OPEN IMAEG";
        return -1;
    }

    while(1){
        imshow("image", src);
        char c = waitKey(0);

        if(c == 27){
            break;
        }
        else if(c == 'i'){
            pyrUp(src, src, Size(src.cols * 2, src.rows * 2));
            cout << "UpSampling\n";
        }
        else if(c == 'o'){
            pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
            cout << "DownSampling\n";
        }
    }
    destroyAllWindows();
    return 0;
}