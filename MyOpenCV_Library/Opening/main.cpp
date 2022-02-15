#include<iostream>
#include<opencv2/opencv.hpp>


cv::Mat myDilation(cv::Mat input_image){
    cv::Mat mask;
    cv::Mat label_image = input_image.clone();
    bool match = false;

    mask = cv::Mat::ones(3, 3, CV_8UC1);
    /*     mask 
        [[1, 1, 1]
         [1, 1, 1]
         [1, 1, 1]] */

    for(int y=1; y<input_image.rows-1; ++y){
        for(int x=1; x<input_image.cols-1; ++x){
            // Dilation
            for(int j=-1; j<=1; ++j){
                for(int i=-1; i<=1; ++i){
                    if((int)input_image.at<uchar>(y+j, x+i) > 0 && (int)mask.at<uchar>(j+1, i+1) > 0){
                        match = true;
                        break;
                    }
                }
                if(match) break;
            }
            if(match){
                for(int j=-1; j<=1; ++j){
                    for(int i=-1; i<=1; ++i){
                        label_image.at<uchar>(y+j, x+i) = 255;
                    }
                }
            } else {
                for(int j=-1; j<=1; ++j){
                    for(int i=-1; i<=1; ++i){
                        label_image.at<uchar>(y+j, x+i) = 0;
                    }
                }
            }
            match = false;
        }
    }
    return label_image;
}

cv::Mat myErosion(cv::Mat input_image){
    cv::Mat mask;
    cv::Mat label_image = input_image.clone();
    bool match = true;

    mask = cv::Mat::ones(3, 3, CV_8UC1);
    /*     mask 
        [[1, 1, 1]
         [1, 1, 1]
         [1, 1, 1]] */

    for(int y=1; y<input_image.rows-1; ++y){
        for(int x=1; x<input_image.cols-1; ++x){
            // Erosion
            for(int j=-1; j<=1; ++j){
                for(int i=-1; i<=1; ++i){
                    if((int)input_image.at<uchar>(y+j, x+i) == 0 && (int)mask.at<uchar>(j+1, i+1) > 0){
                        match = false;
                        break;
                    }
                }
                if(!match) break;
            }
            if(match){
                for(int j=-1; j<=1; ++j){
                    for(int i=-1; i<=1; ++i){
                        label_image.at<uchar>(y+j, x+i) = 255;
                    }
                }
            } else {
                for(int j=-1; j<=1; ++j){
                    for(int i=-1; i<=1; ++i){
                        label_image.at<uchar>(y+j, x+i) = 0;
                    }
                }
            }
            match = true;
        }
    }
    return label_image;
}



int main(int argc, char** argv){
    if(argc != 2){
        std::cout << "Usage: ./opening <YOUR IMAGE>\n";
        return 1;
    }
    char *image_path = argv[1];
    
    cv::Mat result;
    cv::Mat image = cv::imread(image_path);
    if(image.empty()){
        std::cout << "CANNOT OPEN IMAGE!\n";
        return -1;
    }

    // make gray image
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::threshold(image, image, 200, 255, cv::THRESH_BINARY_INV);

    // make salt and pepper noise in image
    int x = 0;
    int y = 0;
    int noise = 0;

    for(int i=0; i<900; i++){
        y = std::rand() % image.rows;
        x = std::rand() % image.cols;
        noise = std::rand() % 256;
        image.at<uchar>(y, x) = noise;
    }


    cv::imshow("origin image", image);

    result = myErosion(image);
    result = myDilation(result);
    cv::imshow("Erosion Image", result);

    cv::waitKey(0);
    return 0;
}