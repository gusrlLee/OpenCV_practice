#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;

void update_map(int &ind, Mat &map_x, Mat &map_y);

int main(){
    Mat src = imread("../../data/lena.png");
    if(src.empty()){
        std::cout << "CANNOT OPEN IMAEGE";
        return -1;
    }

    Mat dst(src.size(), src.type());
    Mat map_x(src.size(), CV_32FC1);
    Mat map_y(src.size(), CV_32FC1);

    const char* remap_window = "remap Demo";
    namedWindow(remap_window);

    int ind = 0;
    for(;;){
        update_map(ind, map_x, map_y);
        remap(src, dst, map_x, map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
        /*
        remap :  process of taking pixels from one place in the image and locating them in another position in a new image
        src : input image 
        dst : Destination image of same size as src
        map_x : The mapping function in the x direction. It is equivalent to the first component of h(i, j)
        map_y : Same as above, but in y direction. Not that map_y and map_x are both of the same size src
        INTER_LINEAR : The type of interpolation to use for non-integer pixels. This is by default
        BORDER_CONSTANT: Default
        */
        imshow(remap_window, dst);
        char c = (char)waitKey(0);
        if(c == 27){
            break;
        }
    }

    return 0;
}

void update_map(int &ind, Mat &map_x, Mat &map_y){
    for( int i = 0; i < map_x.rows; i++ ){
        for( int j = 0; j < map_x.cols; j++ ){
            switch( ind )
            {
                case 0:
                    if( j > map_x.cols*0.25 && j < map_x.cols*0.75 && i > map_x.rows*0.25 && i < map_x.rows*0.75 )
                    {
                        map_x.at<float>(i, j) = 2*( j - map_x.cols*0.25f ) + 0.5f;
                        map_y.at<float>(i, j) = 2*( i - map_x.rows*0.25f ) + 0.5f;
                    }
                    else
                    {
                        map_x.at<float>(i, j) = 0;
                        map_y.at<float>(i, j) = 0;
                    }
                    break;

                case 1:
                    map_x.at<float>(i, j) = (float)j;
                    map_y.at<float>(i, j) = (float)(map_x.rows - i);
                    break;

                case 2:
                    map_x.at<float>(i, j) = (float)(map_x.cols - j);
                    map_y.at<float>(i, j) = (float)i;
                    break;

                case 3:
                    map_x.at<float>(i, j) = (float)(map_x.cols - j);
                    map_y.at<float>(i, j) = (float)(map_x.rows - i);
                    break;

                default:
                    break;
            } // end of switch
        }
    }
    ind = (ind+1) % 4;
}
