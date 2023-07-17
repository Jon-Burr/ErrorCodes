cmake_minimum_required(VERSION 3.12)
project(ErrorCodes VERSION 0.0.0)

add_library(ErrorCodes INTERFACE)
target_include_directories(ErrorCodes INTERFACE include)
target_compile_features(ErrorCodes INTERFACE cxx_std_20)