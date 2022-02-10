#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat image = imread("../../data/cards.png");

    if(image.empty()){
        cout << "CANT OPEN IMAGE";
        return -1;
    }

    // Show origin image
    imshow("image", image);
    // change the background from white to black, since that will help later to extract
    // better results during the use of Distance Transform
    Mat mask;
    inRange(image, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
    image.setTo(Scalar(0, 0, 0), mask);

    // show ouput image 
    imshow("Black Background image", image);

    // create a kernel that we will use to sharpen our image
    Mat kernel = (Mat_<float>(3,3) << 
                1, 1, 1,
                1, -8, 1,
                1, 1, 1); // an approximation of second derivative, a quite strong kernel
    
    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CV_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    Mat imgLaplacian;
    filter2D(image, imgLaplacian, CV_32F, kernel);
    Mat sharp;
    image.convertTo(sharp, CV_32F);
    Mat imgResult = sharp - imgLaplacian;

    // convert back to 8bits gray scale image
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

    imshow("Laplace image", imgLaplacian);
    imshow("new sharped image", imgResult);

    // create binary image from source image
    Mat bw;
    cvtColor(imgResult, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("Binary Image", bw);

    // Perform the distance transform algorithm
    Mat dist;
    distanceTransform(bw, dist, DIST_L2, 3);

    // Nomalize the distance image for range
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    imshow("distance Transform image", dist);

    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);

    // Dilate a bit the dist image
    Mat kernel1 = Mat::zeros(3, 3, CV_8U);
    dilate(dist, dist, kernel1);
    imshow("Peaks", dist);

    // Create the CV_8U version of the distance image
    // It is needed for findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);

    // Find total markers
    vector<vector<Point>> contours;
    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // create the marker image for the watershed algroithm
    Mat markers = Mat::zeros(dist.size(), CV_32S);

    // Draw the foreground markers
    for(size_t i=0; i<contours.size(); i++){
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
    }

    // Draw the background markers
    circle(markers, Point(5, 5), 3, Scalar(255), -1);
    Mat markers8u;
    markers.convertTo(markers8u, CV_8U, 10);
    imshow("Markers", markers8u);

    watershed(imgResult, markers);

    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);
    imshow("Markers_v2", mark);

    // Generate random colors
    vector<Vec3b> colors;
    for(size_t i=0; i<contours.size(); i++){
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);

        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);

    // fill labeled objects with random colors
    for(int i=0; i<markers.rows; i++){
        for(int j=0; j<markers.cols; j++){
            int index = markers.at<int>(i, j);
            if(index > 0 && index <= static_cast<int>(contours.size())){
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
        }
    }

    imshow("Final result", dst);
    waitKey();
    return 0;
}