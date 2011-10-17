/**
 * @file proj3-1-1.cpp
 * @brief usage: ./proj3-1-1 pts2d-pic_a.txt pts3d.txt
 * @author A. Huaman
 * @date 2011-10-17
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

using namespace cv;
/**
 * @function main 
 */
int main( int argc, char* argv[] )
{
  cv::Mat T2( 3, 3, CV_32FC1);
  cv::Mat s( 3, 3, CV_32FC1);
  s.at<float>(0,0) = 0.00260383;   s.at<float>(0,1) = 0;   s.at<float>(0,2) = 0; 
  s.at<float>(1,0) = 0 ;   s.at<float>(1,1) = 0.00260383;   s.at<float>(1,2) = 0; 
  s.at<float>(2,0) = 0 ;   s.at<float>(2,1) = 0 ;   s.at<float>(2,2) = 1; 

  cv::Mat c( 3, 3, CV_32FC1);
  c.at<float>(0,0) = 1;   c.at<float>(0,1) = 0;   c.at<float>(0,2) = -558.95; 
  c.at<float>(1,0) = 0 ;   c.at<float>(1,1) = 1;  c.at<float>(1,2) = -325.6; 
  c.at<float>(2,0) = 0 ;   c.at<float>(2,1) = 0;  c.at<float>(2,2) = 1.0; 

  gemm( s, c, 1, cv::Mat::zeros(3,3, CV_32FC1), 0, T2 );
  std::cout<< s << std::endl;
  std::cout<< c << std::endl;
  std::cout<< T2 << std::endl;

/*
  cv::Mat p( 3, 3, CV_32FC1);
  p.at<float>(0,0) = -0.458;   p.at<float>(0,1) = 0.2995;   p.at<float>(0,2) = 0.014; 
  p.at<float>(1,0) = 0.051 ;   p.at<float>(1,1) = 0.055;   p.at<float>(1,2) = 0.541; 
  p.at<float>(2,0) = -0.109 ;   p.at<float>(2,1) = -0.178 ;   p.at<float>(2,2) = 0.044; 

  cv::Mat pinv(3,3, CV_32FC1);

  std::cout << p << std::endl;
  pinv = p.inv();

  std::cout<< " Pinv: " << std::endl;
  std::cout << pinv << std::endl;

  p = p.inv();
  std::cout<< " P inverted in same matrix: " << std::endl;
  std::cout << p << std::endl;
   return 0; 
*/
}

