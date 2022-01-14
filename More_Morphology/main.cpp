#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

void show_wait_destroy(const char* window_name, cv::Mat image);

int main(){
    Mat src, gray;

    src = imread("../image.png");
    resize(src, src, Size(300, 300));
    if(src.empty()){
        cout << "CANNOT OPEN IMAEG\n";
        return -1;
    }

    imshow("origin", src);
    if(src.channels() == 3)
        cvtColor(src, gray, COLOR_BGR2GRAY);
    else
        gray = src;

    // show gray image
    show_wait_destroy("gray", gray);
    
    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    
    // show binary image
    show_wait_destroy("binary", bw);

    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();

    // Specify size on horizontal axis
    int horizontal_size = horizontal.cols / 30;

    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontal_structure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));

    // Apply Morphology operation
    erode(horizontal, horizontal, horizontal_structure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontal_structure, Point(-1, -1));

    // Show extracted horizontal lines
    show_wait_destroy("horizontal", horizontal);

    // Specify size on vertical axis
    int vertical_size = vertical.rows / 30;

    // Create structure element for extracting vertical lines through morphology operations
    Mat vertical_structure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));

    // Apply morphology operation
    erode(vertical, vertical, vertical_structure, Point(-1, -1));
    dilate(vertical, vertical, vertical_structure, Point(-1, -1));

    // show extracted vertical lines
    show_wait_destroy("vertical", vertical);

    // Inverse vertical image
    bitwise_not(vertical, vertical);
    show_wait_destroy("vertical_bit", vertical);

    // Extract edges and smooth image according to the logic
    // 1. extract edges
    // 2. dilate(edges)
    // 3. src.copyTo(smooth)
    // 4. blur smooth image
    // 5. smooth.copyTo(src, edges)

    // Step 1
    Mat edges;
    adaptiveThreshold(vertical, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    show_wait_destroy("edges", edges);

    // Step 2
    Mat kernel = Mat::ones(2, 2, CV_8UC1);
    dilate(edges, edges, kernel);
    show_wait_destroy("dilate", edges);

    // Step 3
    Mat smooth;
    vertical.copyTo(smooth);

    // Step 4
    blur(smooth, smooth, Size(2, 2));

    // Step 5
    smooth.copyTo(vertical, edges);

    // Show final result
    show_wait_destroy("final", vertical);

    return 0;
}

void show_wait_destroy(const char* window_name, cv::Mat image){
    imshow(window_name, image);
    moveWindow(window_name, 500, 0);
    waitKey(0);
    destroyWindow(window_name);
}