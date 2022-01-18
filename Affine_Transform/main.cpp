#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat image = imread("../../data/lena.png");
    if(image.empty()){
        cout << "CANNOT OPENED IMAEG";
        return -1;
    }
    Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(image.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, image.rows - 1.f);

    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, image.rows*0.33f); 
    dstTri[1] = Point2f(image.cols*0.85f, image.rows*0.25f); 
    dstTri[2] = Point2f(image.cols*0.15f, image.rows*0.7f); 

    Mat wrap_mat = getAffineTransform(srcTri, dstTri);
    Mat wrap_dst = Mat::zeros(image.rows, image.cols, image.type());

    warpAffine(image, wrap_dst, wrap_mat, wrap_dst.size());

    Point center = Point(wrap_dst.cols/2, wrap_dst.rows/2);
    double angle = -50.0;
    double scale = 0.6;
    Mat rot_mat = getRotationMatrix2D( center, angle, scale );

    Mat wrap_rotate_dst;
    warpAffine(wrap_dst, wrap_rotate_dst, rot_mat, wrap_dst.size());

    imshow("image", image);
    imshow("wrap matrix", wrap_dst);
    imshow("Rotate", wrap_rotate_dst);

    waitKey(0);
    return 0;
}