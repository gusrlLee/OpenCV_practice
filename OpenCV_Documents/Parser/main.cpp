#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <vector>

const char* keys = {" \\ 
    {help h ?               |         | this is help. } \\ 
    {camera-path            | nullptr | Write your Camera Path. } \\
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
    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("help")) {
        help();
        return 0;
    }

    if (argc < 2) {
        help();
        return 0;
    }

    std::string camera_path = parser.get<std::string>("camera-path");
    std::string lidar_path = parser.get<std::string>("lidar-path");
    std::string arduino_path = parser.get<std::string>("arduino-path");

    std::cout << "camera path = " << camera_path << std::endl;
    std::cout << "Lidar path = " << lidar_path << std::endl;
    std::cout << "arduino path = " << arduino_path << std::endl;

    return 0;
}
