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

    void RansacSimilarity( std::vector<cv::KeyPoint> _points1, 
												   std::vector<cv::KeyPoint> _points2,    
                           std::vector< cv::DMatch > _matches );
    cv::Mat drawSuperposedSimilarity();
    cv::Mat drawConsensusSet( cv::Mat _img1, cv::Mat _img2 ); 
    int getRandom( int _numElem );


    std::vector<cv::Point2f> mSimilaritySet1;
    std::vector<cv::Point2f> mSimilaritySet2;

    int mSimilarity_TotalSamplesNum;
    float mSimilarity_e;
    int mSimilarity_N;
    float mSimilarity_P;
    int mSimilarity_s;
    int mBestSampleIndex1;
    int mBestSampleIndex2;

    std::vector<int> mConsensusIndices; 

    float mSampleX; float mSampleY;
    float mTestX; float mTestY;
    float mError;
    float mThresh2;

    int mBestCount;
    float mBest_a; float mBest_b; 
    float mBest_c; float mBest_d; 

    int mRows1; int mCols1;
    int mRows2; int mCols2;
    cv::Mat mConsensusImage;

   cv::RNG rng;
   cv::SVD svd;
   cv::Mat mA;
   cv::Mat mB;
   cv::Mat mParam;
   float mSample_a;    float mSample_b;    float mSample_c;   float mSample_d;

};

#endif /** _RANSAC_H_ */
