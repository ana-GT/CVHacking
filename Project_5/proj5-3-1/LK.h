/**
 * @file LK.h
 * @date 2011-11-18
 * @author A. Huaman
 */
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

#ifndef _LK_H_
#define _LK_H_

/**
 * @class LK
 */
class LK {

public:
    LK();
    ~LK();

    // With SVD
    void OpticFlowEstimation1( const cv::Mat &_leftImg, 
                              const cv::Mat &_rightImg, 
                              cv::Mat &_u,
                              cv::Mat &_v );
    // With Cramer
    void OpticFlowEstimation2( const cv::Mat &_leftImg, 
                              const cv::Mat &_rightImg, 
                              cv::Mat &_u,
                              cv::Mat &_v );
    // With regular form - OpenCV style
    void OpticFlowEstimation3( const cv::Mat &_leftImg, 
                              const cv::Mat &_rightImg, 
                              cv::Mat &_u,
                              cv::Mat &_v, 
                              int _level );
    void DrawMotionArrows( cv::Mat &_mu, 
                           cv::Mat &_mv );
    void DrawMotionArrows2( cv::Mat &_mu, 
                            cv::Mat &_mv );
    void DrawMotionArrows3( cv::Mat U, 
                            cv::Mat V, 
                            cv::Mat &_mu, 
                            cv::Mat &_mv );
    void DrawHorArrow( cv::Mat &_img, 
                       cv::Point _p, 
                       int _orient,  
                       double _length,
                       cv::Scalar _color );

    void DrawVerArrow( cv::Mat &_img, 
                       cv::Point _p, 
                       int _orient,  
                       double _length,
                       cv::Scalar _color );

   cv::Mat Remap2to1( cv::Mat _img2, cv::Mat _img1, cv::Mat _vel_x, cv::Mat _vel_y );
   cv::Mat Remap1to2( cv::Mat _img1, cv::Mat _vel_x, cv::Mat _vel_y );
   cv::Mat GetDiff( const cv::Mat &_img2, const cv::Mat &_img1 );

   cv::Mat mIx, mIy;
   cv::Mat mIxx, mIyy, mIxy;
   cv::Mat mIt, mIxt, mIyt;
   cv::Mat mImg1, mImg2;
   cv::Mat mRemap;
   cv::Mat mImg1_16, mImg2_16;
   cv::Mat mIsxx, mIsyy, mIsxy, mIsxt, mIsyt;
   cv::Mat mKernel;
   int mSize;
   int mRows;
   int mCols;
   cv::Mat mU, mV;
};


#endif /** _LK_H_ */
