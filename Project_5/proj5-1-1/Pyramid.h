/**
 * @file Pyramid.h
 * @date 2011-11-15
 * @author A. Huaman
 */
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

#ifndef _PYRAMID_H_
#define _PYRAMID_H_

/**
 * @class Pyramid
 */
class Pyramid {

public:
    Pyramid();
    ~Pyramid();
    cv::Mat Reduce( const cv::Mat &_input, int _windowSize );
    cv::Mat Expand( const cv::Mat &_input, int _windowSize );

};


#endif /** _PYRAMID_H_ */

