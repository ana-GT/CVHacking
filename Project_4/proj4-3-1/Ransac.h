/**
 * @file Ransac.h
 */

#ifndef _RANSAC_H
#define _RANSAC_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <math.h>

/**
 * @class Ransac
 */
class Ransac {

public:
    Ransac();
    ~Ransac();

    void RansacTranslation( std::vector<cv::KeyPoint> _points1, 
												    std::vector<cv::KeyPoint> _points2,    
                            std::vector< cv::DMatch > _matches );
    cv::Mat drawSuperposedTranslation();
    cv::Mat drawConsensusSet( cv::Mat _img1, cv::Mat _img2 ); 
    int getRandom( int _numElem );


    std::vector<cv::Point2f> mTranslationSet1;
    std::vector<cv::Point2f> mTranslationSet2;

    int mTranslation_TotalSamplesNum;
    float mTranslation_e;
    int mTranslation_N;
    float mTranslation_P;
    int mTranslation_s;
    int mBestSampleIndex;

    std::vector<float> mConsensusSetDX;
    std::vector<float> mConsensusSetDY;
    std::vector<int> mConsensusIndices; 

    float mSampleDX; float mSampleDY;
    float mTestDX; float mTestDY;
    float mError;
    float mThresh2;

    int mBestCount;
    float mBestDX; 
    float mBestDY;

    int mRows1; int mCols1;
    int mRows2; int mCols2;
    cv::Mat mConsensusImage;

   cv::RNG rng;

};

#endif /** _RANSAC_H_ */
