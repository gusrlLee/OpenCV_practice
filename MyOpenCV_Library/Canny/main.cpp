#include<iostream>
#include<opencv2/opencv.hpp>
#include<cmath>

#define MASK_SIZE 3
#define PI 3.14
#define SIGMA 0.8

static int directionQuantization(float direction){
    float temp = 0;
    if(direction > 0){
        temp = direction;
    } else {
        temp = 360 + direction;
    }


    if(temp < 45){
        return 0;
    } else if(temp < 90) {
        return 1;
    } else if(temp < 135) {
        return 2;
    } else if(temp < 180) {
        return 3;
    } else if(temp < 225) {
        return 4;
    } else if(temp < 270) {
        return 5;
    } else if(temp < 315) {
        return 6;
    } else {
        return 7;
    }

}

cv::Mat paddingImage(cv::Mat src){
    cv::Mat result = cv::Mat::zeros(src.rows+2, src.cols+2, CV_8UC1);
    for(int row=0; row<src.rows; row++){
        for(int col=0; col<src.cols; col++){
            result.at<uchar>(row+1, col+1) = src.at<uchar>(row, col);
        }
    }
    return result;

}

int main(int argc, char** argv){
    float sum = 0;

    // make gray image
    float mask[3][3] = {0.0};

    cv::Mat padding_image;
    cv::Mat image = cv::imread("../../../data/lena.jpg");
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    float bot = 2 * PI * SIGMA * SIGMA;
    float pow_sigma = SIGMA * SIGMA;

    // Make Gaussian Mask
    for(int row=-1; row<2; row++){
        for(int col=-1; col<2; col++){
            mask[row+1][col+1] = exp(-(col*col + row*row) / (2 * pow_sigma)) / bot;
        }
    }

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            std::cout  << mask[i][j] << " ";
            sum += mask[i][j];
        }
        printf("\n");
    }
    printf("filter sum is : %.3f", sum);

    std::cout << "image size" << image.size() << "\n";

    // padding image for Gaussian blur operation
    padding_image = paddingImage(image);
    std::cout << "padding image size :" << padding_image.size() << "\n";

    // Blur step
    float temp;
    cv::Mat blured_image = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    for(int row=1; row<padding_image.rows-1; row++){
        for(int col=1; col<padding_image.cols-1; col++){
            for(int y=-1; y<2; y++){
                for(int x=-1; x<2; x++){
                    temp += (padding_image.at<uchar>(row - y, col - x) * mask[y+1][x+1]);
                }
            }
            blured_image.at<uchar>(row-1, col-1) = (uchar)temp;
            temp = 0.0;
        } 
    }

    // define Sobel operation masks
    int m_y[3][3] = {{-1, -2, -1},
                     {0, 0, 0},
                     {1, 2, 1}};

    int m_x[3][3] = {{-1, 0, 1},
                     {-2, 0, 2},
                     {-1, 0, 1}};
    float buf_x = 0;
    float buf_y = 0;
    // After zero-padding image, Start Sobel operation edge strength and edge direction
    blured_image = paddingImage(blured_image);
    cv::Mat sobel_edge_image = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

    cv::Mat edge_direction = cv::Mat::zeros(image.rows, image.cols, CV_32FC1);
    for(int ii=1; ii<blured_image.rows-2; ii++){
        for(int jj=1; jj<blured_image.cols-2; jj++){
            buf_y = blured_image.at<uchar>(ii-1, jj-1) * (-1) + 
                blured_image.at<uchar>(ii-1, jj) * (-2) + 
                blured_image.at<uchar>(ii-1, jj+1) * (-1) + 
                blured_image.at<uchar>(ii+1, jj-1) * (1) + 
                blured_image.at<uchar>(ii+1, jj) * (2) + 
                blured_image.at<uchar>(ii+1, jj+1) * (1);
            
            buf_x = blured_image.at<uchar>(ii-1, jj-1) * (-1) + 
                blured_image.at<uchar>(ii, jj-1) * (-2) + 
                blured_image.at<uchar>(ii+1, jj-1) * (-1) + 
                blured_image.at<uchar>(ii-1, jj+1) * (1) + 
                blured_image.at<uchar>(ii, jj+1) * (2) + 
                blured_image.at<uchar>(ii+1, jj+1) * (1);

            float buf_xy = sqrt(buf_y * buf_y + buf_x * buf_x);
            edge_direction.at<float>(ii-1, jj-1) = atan(buf_y / buf_x);
            sobel_edge_image.at<uchar>(ii-1, jj-1) = (uchar)buf_xy;
        }
    }

    // Edge Direction Quantization -> non-maximum suppression
    float a=0, b=0;
    for(int ii=1; ii<sobel_edge_image.rows-2; ii++){
        for(int jj=1; jj<sobel_edge_image.cols-2; jj++){
            int direct = directionQuantization(edge_direction.at<float>(ii, jj));
            if(direct == 0 || direct == 4){
                a = sobel_edge_image.at<uchar>(ii, jj-1);
                b = sobel_edge_image.at<uchar>(ii, jj+1);
            } else if(direct == 1 || direct == 5){
                a = sobel_edge_image.at<uchar>(ii-1, jj-1);
                b = sobel_edge_image.at<uchar>(ii+1, jj+1);
            } else if(direct == 2 || direct == 6){
                a = sobel_edge_image.at<uchar>(ii+1, jj);
                b = sobel_edge_image.at<uchar>(ii+1, jj);
            } else {
                a = sobel_edge_image.at<uchar>(ii+1, jj+1);
                b = sobel_edge_image.at<uchar>(ii-1, jj-1);
            }

            if(sobel_edge_image.at<uchar>(ii,jj) < a || sobel_edge_image.at<uchar>(ii,jj) < b){
                sobel_edge_image.at<uchar>(ii,jj) = 0; 
            }
        } 
    }

    // Thresholding image
    int threshold = 100;
    cv::Mat canny_edge_image = cv::Mat::zeros(sobel_edge_image.rows, sobel_edge_image.cols, CV_8UC1);
    for(int ii=0; ii<sobel_edge_image.rows; ii++){
        for(int jj=0; jj<sobel_edge_image.cols; jj++){
            if(sobel_edge_image.at<uchar>(ii, jj) > threshold){
                canny_edge_image.at<uchar>(ii, jj) = sobel_edge_image.at<uchar>(ii, jj); 
            }
        }
    }
    // show image
    imshow("image", image);
    imshow("canny", canny_edge_image);
    cv::waitKey(0);

    return 0;
}
