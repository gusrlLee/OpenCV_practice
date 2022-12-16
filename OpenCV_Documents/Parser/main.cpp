#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <vector>

const char* keys = {" \\ 
    {help h ?               |         | this is help. } \\ 
    {camera-path            | nullptr | Write your Camera Path. } \\
    {focal-length           | 0       | Write focal length your camera } \\ 
    {cx                     | 0       | Write principal point your camera } \\
    {cy                     | 0       | Write your camera principal point } \\
    {lidar-path             | nullptr | Write your LIDAR Path. } \\
    {arduino-path           | nullptr | Write your Arduino Path.}"};

void help() {
        std::cout << "[SYSTEM INFORMATION]\n"
                 "--------------------------------------------------------------------------------------------------------\n"
                 "You need to write argument for this program. \n"
                 "If you want simulation mode, you need to write your dataset path for simulation.\n"
                 "If you want real mode, you need to write your camera path\n\n"
                 "[Arguments]\n"
                 "camera-path = Your Camera path or dataset path. check your connect status. (defualt : NULL)\n"
                 "lidar-path = Your LIDAR path. check your connect status. (default : NULL)\n"
                 "Arduino-path = Your Arduino path. check your connect status. (default : NULL)\n"
                 "\n[EXAMPLE]\n"
                 "ex) ./auto --camera-path=[YOUR PATH] --lidar-path=[YOUR PATH] --Arduino-path=[YOUR PATH]\n"
                 "--------------------------------------------------------------------------------------------------------\n" << std::endl;
}

int main(int argc, char* argv[]) {
//    cv::CommandLineParser parser(argc, argv, keys);
//
//    if (parser.has("help")) {
//        help();
//        return 0;
//    }
//
//    if (argc < 2) {
//        help();
//        return 0;
//    }
//
//    std::string lidar_path;
//
//    std::string camera_path = parser.get<std::string>("camera-path");
//    double focal_length = parser.get<double>("focal-length");
//
//    if (parser.has("lidar-path"))
//        lidar_path = parser.get<std::string>("lidar-path");
//
//
//    std::cout << "camera path = " << camera_path << std::endl;
//    std::cout << "Lidar path = " << lidar_path << std::endl;
//    std::cout << "focal-length = " << focal_length << std::endl;
//
//    std::cout << "string size " << lidar_path << std::endl;
//    if (lidar_path.compare("nullptr") == 0) { 
//        std::cout << "NULL ptr !" << std::endl;
//    }
//
    cv::VideoCapture cap("../highway.mp4");

    if (cap.isOpened()) {
        std::cout << "Fail to open camera\n";
        return -1;
    }

    cv::Mat frame;
    while (1) {
        cap >> frame;
        if (frame.empty()) break;

        cv::imshow("window", frame);
        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    cap.release();

    return 0;
}
