#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void PointOp();
void SizeOp();
void RectOp();
// void RotatedRectOp();
// void RangeOp();
// void StringOp();

int main(){
    PointOp();
    SizeOp();
    RectOp();
    // RotatedRectOp();
    // RangeOp();
    // StringOp();
    
    return 0; 
}

void PointOp(){
    Point pt1;
    pt1.x = 5;
    pt1.y = 10;

    Point pt2(10, 30);
    Point pt3 = pt1 + pt2;

    Point pt4 = pt3 * 3;
    
    int d21 = pt1.dot(pt2);

    cout << "pt1 = " << pt1 << "\n";
    cout << "pt2 = " << pt2 << "\n";
}

void SizeOp(){
    Size sz1, sz2(10, 20);
    sz1.width = 5;
    sz1.height = 10; // sz1 = (5, 10)

    Size sz3 = sz1 + sz2;
    Size sz4 = sz1 * 2;

    cout << "sz1 = " << sz1 << "\n";
    cout << "sz2 = " << sz2 << "\n";
}

void RectOp(){
    Rect rc1;
    Rect rc2(10, 10, 60, 40);

    Rect rc3 = rc1 + Size(50, 40);
    Rect rc4 = rc2 + Point(10, 10);

    Rect rc5 = rc1 & rc2;
    Rect rc6 = rc3 | rc4;

    cout << "rc5 = " << rc5 << "\n";
    cout << "rc6 = " << rc6 << "\n";
}



