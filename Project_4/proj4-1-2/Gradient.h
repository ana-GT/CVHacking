/**
 * @file Gradient.h
 * @author A. Huaman
 */

#ifndef _GRADIENT_H_
#define _GRADIENT_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

/**
 * @class Gradient
 */
class Gradient {

  public:
  Gradient( );
  void Init( const cv::Mat &_img );

  ~Gradient();
   
  void GetGradients();

  cv::Mat GetImageGradientX();
  cv::Mat GetImageGradientY();
  cv::Mat GetImageGradientStitched();
  cv::Mat GetImageGradientXY();
  cv::Mat GetImageGradientYX();
  cv::Mat GetImageGradientXX();
  cv::Mat GetImageGradientYY();

  cv::Mat GetHarris( double _coeff );
  cv::Mat GetImageHarris();

  private:
  cv::Mat GetGradientX();
  cv::Mat GetGradientY();
  cv::Mat GetGradientXY();
  cv::Mat GetGradientYX();
  cv::Mat GetGradientXX();
  cv::Mat GetGradientYY();

  cv::Mat GetWindow( int _y, int _x, const cv::Mat &_img, int _size );
  int SobelX( const cv::Mat &_wx );
  int SobelY( const cv::Mat &_wy );
  int GetWeightedSum( const cv::Mat &_window, const cv::Mat &_weight );
  cv::Mat PadImage( const cv::Mat &_img, int _pad );


  cv::Mat mImage;
  cv::Mat mPadImage;
  int mRows;
  int mCols;
  int mPad;

  cv::Mat mMaskX;
  cv::Mat mMaskY;

  cv::Mat mGradientX;
  cv::Mat mGradientY;
  cv::Mat mGradientXY;
  cv::Mat mGradientYX;
  cv::Mat mGradientXX;
  cv::Mat mGradientYY;

  cv::Mat mPadGradientX;
  cv::Mat mPadGradientY;

  double mCoeff;
  cv::Mat mHarris;
  cv::Mat mWeightH;
  
  cv::Mat mImageGradientX;
  cv::Mat mImageGradientY;
  cv::Mat mImageGradientStitched;
  cv::Mat mImageGradientXY;
  cv::Mat mImageGradientYX;
  cv::Mat mImageGradientXX;
  cv::Mat mImageGradientYY;
  cv::Mat mImageHarris;

};

#endif /** _GRADIENT_H_ */
