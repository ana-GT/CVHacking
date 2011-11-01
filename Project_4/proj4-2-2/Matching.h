/**
 * @file Matching.h
 * @author A. Huaman
 */

#ifndef _MATCHING_H_
#define _MATCHING_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include "Gradient.h"

/**
 * @class Matching 
 */
class Matching
{
   public:
   Matching();
   ~Matching();
   void Init_Pair1( const cv::Mat &_img1, 
												   double _coeff1, 
                           int _thresh1, 
                           int _radius1 ); 

   void Init_Pair2( const cv::Mat &_img2, 
												   double _coeff2, 
                           int _thresh2, 
                           int _radius2 ); 

   void buildSIFTDescriptor1();
   void buildSIFTDescriptor2();

   cv::Mat BruteMatcher();
   cv::Mat DrawGoodMatches();
   cv::Mat DrawMatches(); 
   cv::Mat Surfie();

   Gradient mG1;
   Gradient mG2;

   cv::Mat mMatchesImage;
   cv::Mat mGoodMatchesImage;
   std::vector< cv::DMatch > mMatches;
   std::vector< cv::DMatch > mGoodMatches;

   cv::Mat DrawOrient1();
   cv::Mat DrawOrient2();
   cv::Mat DrawOrient1Handy();
   cv::Mat DrawOrient2Handy();

   cv::Mat mPair1; /**< Grayscale */
   cv::Mat mPair2; /**< Grayscale */

   cv::Mat mColor1; /**< */
   cv::Mat mColor2;

   int mKeyPoints1Num;
   int mKeyPoints2Num;
   std::vector<cv::Point2f> mPoints1;
   std::vector<cv::Point2f> mPoints2;

   std::vector<cv::KeyPoint> mKeyPoints1;
   std::vector<cv::KeyPoint> mKeyPoints2;

   cv::Mat mAngles1;
   cv::Mat mAngles2;
  
   int mRows1; int mCols1;
   int mRows2; int mCols2;

   double mHarrisCoeff1; double mHarrisCoeff2;
   int mThresh1; int mThresh2;
   int mRadius1; int mRadius2;

   cv::Mat mDescriptor1;
   cv::Mat mDescriptor2;

   cv::RNG rng;
};


#endif /** _MATCHING_H_ */
