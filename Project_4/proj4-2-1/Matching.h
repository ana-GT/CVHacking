/**
 * @file Matching.h
 * @author A. Huaman
 */

#ifndef _MATCHING_H_
#define _MATCHING_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <math.h>

/**
 * @class Matching 
 */
class Matching
{
   public:
   Matching();
   ~Matching();
   void Init_Pair1( const cv::Mat &_img1_color, const cv::Mat &_img1, std::vector<cv::Point> &_keypoints1, const cv::Mat &_gx1, const cv::Mat &_gy1 );
   void Init_Pair2( const cv::Mat &_img2_color, const cv::Mat &_img2, std::vector<cv::Point> &_keypoints2, const cv::Mat &_gx2, const cv::Mat &_gy2 ); 

   cv::Mat DrawOrient1();
   cv::Mat DrawOrient2();

   cv::Mat mPair1;
   cv::Mat mPair2;

   cv::Mat mColor1;
   cv::Mat mColor2;

   int mKeyPoints1Num;
   int mKeyPoints2Num;
   std::vector<cv::Point> mKeyPoints1;
   std::vector<cv::Point> mKeyPoints2;

   cv::Mat mAngles1;
   cv::Mat mAngles2;
  
   int mRows1; int mCols1;
   int mRows2; int mCols2;

   cv::RNG rng;
};


#endif /** _MATCHING_H_ */
