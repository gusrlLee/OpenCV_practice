// use Second Moment Matrix for Harris Corner
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>

int main(int argc, char** argv){
    // image load
    cv::Mat image = cv::imread("../../../data/lena.jpg");
    cv::Mat gray_image;

    if(image.empty()){
        std::cout << "CANNOT OPEN IMAGE\n";
        exit(1);
    }

    // make gray image
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    // make Gaussian_mask
    float gaussian_mask[3][3] = {{0.0625, 0.125, 0.0625}, 
                                  {0.125, 0.25, 0.125},
                                  {0.0625, 0.1238, 0.0625}};

    // print G_matrix
    std::cout << "Gaussian Mask = \n";
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            std::cout << gaussian_mask[i][j] << " ";
        }
        printf("\n");
    }

    int gradi_x[3] = {-1, 0, 1};
    int gradi_y[3] = {-1, 0, 1};
    
    int width = gray_image.size().width;
    int height = gray_image.size().height;

    cv::Mat d_x = cv::Mat::zeros(width, height, CV_32FC1);
    cv::Mat d_y = cv::Mat::zeros(width, height, CV_32FC1);

    for(int row=1; row<gray_image.rows-1; row++){
        for(int col=1; col<gray_image.cols-1; col++){
            float w = float(gray_image.at<uchar>(row ,col+1) - gray_image.at<uchar>(row, col-1));
            float v = float(gray_image.at<uchar>(row+1 ,col) - gray_image.at<uchar>(row-1, col));
            if(w > 0) d_x.at<float>(row, col) = 1.0;
            else if(w < 0) d_x.at<float>(row, col) = -1.0;
            else d_x.at<float>(row, col) = 0;

            if(v > 0) d_y.at<float>(row, col) = 1.0;
            else if(v < 0) d_y.at<float>(row, col) = -1.0;
            else d_y.at<float>(row, col) = 0;
        }
    }

    cv::Mat d_x_2 = d_x.mul(d_x);
    cv::Mat d_y_2 = d_y.mul(d_y);
    cv::Mat d_x_y = d_x.mul(d_y);

    cv::Mat G_d_x_2 = cv::Mat::zeros(width-2, height-2, CV_32FC1);
    cv::Mat G_d_y_2 = cv::Mat::zeros(width-2, height-2, CV_32FC1);
    cv::Mat G_d_x_y = cv::Mat::zeros(width-2, height-2, CV_32FC1);

    float temp1 = 0;
    float temp2 = 0;
    float temp3 = 0;

//    for(int row=1; row<d_x_y.rows-1; row++){
//        for(int col=1; col<d_x_y.cols-1; col++){
//            for(int a=-1; a<2; a++){
//                for(int b=-1; b<2; b++){
//                    temp1 += (float)d_x_2.at<float>(row + a, col + b) * gaussian_mask[a+1][b+1];
//                    temp2 += (float)d_y_2.at<float>(row + a, col + b) * gaussian_mask[a+1][b+1];
//                    temp3 += (float)d_x_y.at<float>(row + a, col + b) * gaussian_mask[a+1][b+1];
//                }
//            }
//            G_d_x_2.at<float>(row, col) = (float)temp1;
//            G_d_y_2.at<float>(row, col) = (float)temp2;
//            G_d_x_y.at<float>(row, col) = (float)temp3;
//            temp1=0; temp2=0; temp3=0;
//        }
//    }

    cv::GaussianBlur(d_x_2, G_d_x_2, cv::Size(7, 7), 0);
    cv::GaussianBlur(d_y_2, G_d_y_2, cv::Size(7, 7), 0);
    cv::GaussianBlur(d_x_y, G_d_x_y, cv::Size(7, 7), 0);

    float k = 0.04f;
    cv::Mat A = cv::Mat::zeros(2, 2, CV_32FC1);
    cv::Mat C = cv::Mat::zeros(width-2, height-2, CV_32FC1);
    
    for(int y=0; y<G_d_x_y.rows; y++){
        for(int x=0; x<G_d_x_y.cols; x++){
            //A.at<float>(0, 0) = d_y_2.at<float>(y, x);
            //A.at<float>(0, 1) = d_x_y.at<float>(y, x);
            //A.at<float>(1, 0) = d_x_y.at<float>(y, x);
            //A.at<float>(1, 1) = d_x_2.at<float>(y, x);

            float p = G_d_y_2.at<float>(y, x);
            float q = G_d_x_2.at<float>(y, x);
            float r = G_d_x_y.at<float>(y, x);

            C.at<float>(y, x) = (p * q - (r*r)) - (k * (p+q) * (p+q));

            float t = (p * q - (r*r)) - k * ((p+q) * (p+q));
        }
    }

    for(int ii=0; ii<C.rows; ii++){
        for(int jj=0; jj<C.cols; jj++){
            float c = C.at<float>(ii,jj);
            if((c > 0.82) && (c > C.at<float>(ii-1, jj) && c > C.at<float>(ii+1, jj) && c > C.at<float>(ii, jj-1) && c > C.at<float>(ii, jj+1))){
                int r = std::rand() % 256;
                int g = std::rand() % 256;
                int b = std::rand() % 256;
                cv::circle(image, cv::Point(jj, ii), 2, cv::Scalar(b, g, r), 1);
            }
        }
    }
    
    // differential
    // cv::imshow("d_x", d_x);
    // cv::imshow("d_y", d_y);

    // differential Matrix^2
    //cv::imshow("d_x^2", d_x_2);
    //cv::imshow("d_y^2", d_y_2);
    //cv::imshow("d_x_y", d_x_y);
    cv::imshow("lena image", image);

    cv::waitKey();
    return 0;
} 
