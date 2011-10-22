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

  Gradient( cv::Mat _img );
  ~Gradient();
   
  int* GetGradientX();
  int* GetGradientY();
  cv::Mat GetImageGradientX();
  cv::Mat GetImageGradientY();
  cv::Mat GetImageGradientXY();

  private:
  int* GetWindow3( int _y, int _x );
  int SobelX( int* _wx );
  int SobelY( int* _wy );

  cv::Mat mImage;
  cv::Mat mPadImage;
  int mRows;
  int mCols;
  int mPad;
  int *mGradientX;
  int *mGradientY;
  cv::Mat mImageGradientX;
  cv::Mat mImageGradientY;
  cv::Mat mImageGradientXY;
  int* mMaskX;
  int* mMaskY;

};

#endif /** _GRADIENT_H_ */
