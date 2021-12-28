#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void InputArrayOP(InputArray _mat);
void printMat();

int main(void){
    InputArrayOP();

    return 0;
}

void InputArrayOP(){
    uchar data1[] = {1, 2, 3, 4, 5, 6};

    Mat mat1(2, 3, CV_8UC1, data1);
    printMat(mat1);

    vector<float> v1 = {1.2f, 3.4f, -2.1f};
    printMat(v1);
}

void printMat(InputArray _mat){
    Mat mat = _mat.getMat();
    cout << "Mat = " << mat << "\n";
}
