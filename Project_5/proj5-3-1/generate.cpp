/**
 * @file proj5-3-1
 * @brief Reduce (Gaussian Pyramid)
 * @author A. Huaman
 */
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>

#include "LK.h"
#include "Pyramid.h"

/**
 * @function main
 */
int main( int argc, char* argv[] ) {

    int rows = 240; 
    int cols = 320;
    cv::Mat im1 = cv::Mat( rows, cols, CV_8UC3 );
    cv::Mat im2 = cv::Mat( rows, cols, CV_8UC3 );

    for( int j = 0; j < rows; j++ ) {
      for( int i = 0; i < cols; i++ ) {
        for( int k = 0; k < 3; k++ ) {
          im1.at<cv::Vec3b>(j,i)[k] = 255;
          im2.at<cv::Vec3b>(j,i)[k] = 255;
        } 
      }
    }

    for( int j = 50; j < 100; j++ ) {
      for( int i = 80; i < 120; i++ ) {
        for( int k = 0; k < 3; k++ ) {
          im1.at<cv::Vec3b>(j,i)[k] = 0;
        } 
      }
    }


    for( int j = 50; j < 100; j++ ) {
      for( int i = 81; i < 121; i++ ) {
        for( int k = 0; k < 3; k++ ) {
          im2.at<cv::Vec3b>(j,i)[k] = 0;
        } 
      }
    }



    /// Write images
    imwrite("test0.png", im1 );    
    imwrite("test1.png", im2 );    

    return 0;

}

