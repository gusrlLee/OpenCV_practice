#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<vector>


int addNoiseFunction(int x){
    return 0.6 * x + 10 + (std::rand() % 30);
}

float f(const float a, const float b, int x){
    return a * x + b;
}

int main(int argc, char** argv){
    cv::Mat image = cv::Mat::zeros(300, 300, CV_8UC1);
    std::vector<cv::Point2i> coordinate_list;
    std::vector<cv::Point3f> inlier_list;
    
    // add Noise 
    for(int x=0; x<300; x++){
        int y = addNoiseFunction(x);
        image.at<uchar>(y, x) = 255;
        coordinate_list.push_back(cv::Point2i(x, y));
    }

    std::cout << "coordinate size = " << coordinate_list.size()  << std::endl;

    // estimation parameters
    float best_a = 0;
    float best_b = 0; 
    float error = 0;
    float min_error = 9999999;

    // test number
    int num = 300; 

    // threshold error 
    float t = 3;

    int max_inlier_count = 0;
    int inlier_counter = 0;
    float sum = 0;

    // RANSAC Algorithm
    for(int i=0; i<num; i++){
        // get random two point
        cv::Point2i p1 = coordinate_list[(std::rand() % 300)];
        cv::Point2i p2 = coordinate_list[(std::rand() % 300)];

        if(p1 == p2) {
            num++;
            continue;
        }

        float gradient = float((p2.y - p1.y)) / float((p2.x - p1.x));
        std::cout << "gradient" << gradient << "\n";
        float y_intercept = p1.y - (p1.x * gradient);

        for(int j=0; j<coordinate_list.size(); j++){
            if(coordinate_list[j] != p1 && coordinate_list[j] != p2){
                float y_hat = f(gradient, y_intercept, coordinate_list[j].x);
                float temp = (y_hat - coordinate_list[j].y);
                if(abs(temp) < t){
                    inlier_counter++;
                }
                sum += (temp * temp);
            }
        }
        // sum is sigma[(x^ - x )^2]
        error = sum / coordinate_list.size(); // divide N

        // extract best model 
        if(inlier_counter > max_inlier_count){
            max_inlier_count = inlier_counter;
            best_a = gradient;
            best_b = y_intercept;
        }

        inlier_counter = 0;
        sum = 0;
        error = 0;
    }

    std::cout << "best a is " << best_a << " best_b is " << best_b << std::endl;
    std::cout << "Max inlier count : " << max_inlier_count << "\n";
    // draw line
    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
    for(int i=0; i<300; i++){
        int out = cvRound(best_a * i + best_b);
        cv::circle(image, cv::Point(i, out), 1, cv::Scalar(0, 0, 255), 1);
    }

    // visualization 
    cv::imshow("image", image);
    cv::waitKey();
    return 0;
}
