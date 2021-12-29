#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void matOp1();
void matOp2();
void matOp3();
void matOp4();
void resizeImage();

int main(){
    //matOp1();
    resizeImage();

    return 0;
}

void matOp1(){
    Mat image1;

    Mat image2(480, 640, CV_8UC1);
    Mat image3(480, 640, CV_8UC3);

    Mat image4(Size(640, 480), CV_8UC1);

    Mat image5(480, 640, CV_8UC1, Scalar(128));
    Mat image6(480, 640, CV_8UC3, Scalar(0, 0, 255));

    Mat mat1 = Mat::zeros(3, 3, CV_32SC1);
    Mat mat2 = Mat::ones(3, 3, CV_32SC1);
    Mat mat3 = Mat::eye(3, 3, CV_32SC1);

    float data[] = {1, 2, 3, 4, 5, 6};
    Mat mat4(2, 3, CV_32FC1, data);

    mat4.create(256, 256, CV_8UC3);

    mat4 = Scalar(255, 0, 0);

    cout << "Mat 4 = " << mat4 << "\n";
}

void matOp2(){
    Mat image1 = imread("../lena.png");
    Mat image2 = image1;

    Mat image3;
    image3 = image1;
    Mat image4 = image1.clone();
    
    Mat image5;
    image1.copyTo(image5);

    imshow("image1", image1);
    imshow("image2", image2);
    imshow("image3", image3);
    imshow("image4", image4);
    imshow("image5", image5);

    waitKey();
    destroyAllWindows();
}

void matOp3(){
    Mat img1 = imread("../lena.png");
    Mat image_resize;
    if(img1.empty()){
        cerr << "image load fail!!\n";
        return;
    }

    cout << "image1 type " << img1.type();
    
    Mat image2 = ~img1;

    imshow("img1", img1);
    imshow("image2", image2);

    waitKey();
    destroyAllWindows();
}

void matOp4(){
    Mat image = imread("../lena.png");

    cout << "Width" << image.cols << "\n";
    cout << "height" << image.rows << "\n";

    if(image.type() == CV_8UC1){
        cout << "image grayscale image\n";
    }
    else if(image.type() == CV_8UC3){
        cout << "image color image\n";
    }

    float data[] = {2.f, 1.414f, 3.f, 1.732f};

    Mat mat1(2, 2, CV_32FC1, data);
    cout << mat1;
}

void resizeImage(){
    Mat orgin_image = imread("../lena.png");
    Mat resize_image;
    imshow("orign image", orgin_image);

    resize(orgin_image, resize_image, Size(384, 384));

    imshow("resize image", resize_image);
    waitKey();
    destroyAllWindows();
}