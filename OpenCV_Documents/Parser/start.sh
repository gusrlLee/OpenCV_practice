#!/bin/bash

echo "Hello world"

CAMERA_PATH="../../data/highway.mp4"
FOCAL_LENGTH="707.0912"

LIDAR_PATH="/dev/ttyUSB0"
LIDAR_BAUDRATE="115200"

ARDUINO_PATH="/dev/ttyUSB1"
ARDUINO_BAUDRATE="115200"


echo "./auto --camera-path=${CAMERA_PATH} --focal-length=${FOCAL_LENGTH} --lidar-path=${LIDAR_PATH} --lidar-baudrate=${LIDAR_BAUDRATE} --arduino-path=${ARDUINO_PATH} --arduino-baudrate=${ARDUINO_BAUDRATE}"


./build/Parsor --camera-path=${CAMERA_PATH} --focal-length=${FOCAL_LENGTH} --lidar-path=${LIDAR_PATH} --lidar-baudrate=${LIDAR_BAUDRATE} --arduino-path=${ARDUINO_PATH} --arduino-baudrate=${ARDUINO_BAUDRATE}

