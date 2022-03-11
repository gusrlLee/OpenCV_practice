#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<math.h>

const float PI = 3.14;

void polarToCartesian(double rho, int theta, cv::Point& p1, cv::Point& p2) {

    int x0 = cvRound(rho * cos(theta));
    int y0 = cvRound(rho * sin(theta));

    p1.x = cvRound(x0 + 1000 * (-sin(theta)));
    p1.y = cvRound(y0 + 1000 * (cos(theta)));

    p2.x = cvRound(x0 - 1000 * (-sin(theta)));
    p2.y = cvRound(y0 - 1000 * (cos(theta)));
}

int main(int argc, char** argv){
    cv::Mat image;
    cv::Mat gray_image;
    cv::Mat edge_image;

    // image load 
    image = cv::imread("../../../data/sudoku.png");
    if(image.empty()){
        std::cout << "CANNOT OPENED IMAGE\n" << std::endl;
        return 1;
    }

    // edge detection 
    cv::resize(image, image, cv::Size(512, 512));
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::blur(gray_image, gray_image, cv::Size(3, 3));
    cv::Canny(gray_image, edge_image, 50, 200, 3);

    // make binary image 0 or 1
    int threshold = 100;

    // 2D array A initialization zeros
    int rho = 0;

    int max_distance = hypot(512, 512);
    int A[724* 2][180] = {0};
    
    for(int y=0; y<edge_image.rows; y++){
        for(int x=0; x<edge_image.cols; x++){
            // if point is edge point, 
            if(edge_image.at<uchar>(y, x) == 255){
                for(int theta=0; theta<=180; theta++){
                    rho = round(y * cos(theta - 90) + x * sin(theta - 90)) + max_distance;
                    std::cout << rho << std::endl;
                    A[rho][theta]++;
                }
            }
        }
    }
    
    cv::Mat output;
    cv::cvtColor(edge_image, output, cv::COLOR_GRAY2BGR);

    std::cout << "line draw ... "<< std::endl;
    for(int i=0; i<2 * max_distance; i++){
        for(int j=0; j<180; j++){
            if(A[i][j] >= 150){
                int rho = i - max_distance;
                int theta = j - 90;
                
                cv::Point p1, p2;
                polarToCartesian(rho, theta, p1, p2);

                cv::line(output, p1, p2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
            } 
        }
    }

    cv::Point dummy1(10, 10), dummy2(100, 100);

    cv::line(output, dummy1, dummy2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);

    cv::imshow("output", output);

    cv::waitKey(0);
    return 0;
}
