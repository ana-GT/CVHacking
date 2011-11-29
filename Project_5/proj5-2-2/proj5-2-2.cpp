/**
 * @file proj5-2-2.cpp
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

/**
 * @function main
 */
int main( int argc, char* argv[] ) {

    /// Read image
    if( argc < 3 )
    { printf("You moron! I need at least two images \n"); return -1; }

    /// Load images grayscale
    cv::Mat img1C = cv::imread( argv[1], 1 );
    cv::Mat img2C = cv::imread( argv[2], 1 );
    cv::Mat img1, img2;

    cvtColor( img1C, img1, CV_BGR2GRAY );
    cvtColor( img2C, img2, CV_BGR2GRAY );

    /// Create flow image
    cv::Mat u, v;
    LK lk;
    lk.OpticFlowEstimation1( img1, img2, u, v );
    //lk.OpticFlowEstimation3( img1, img2, u, v ); // test
    cv::Mat dmu, dmv, dmu2, dmv2;
    lk.DrawMotionArrows( dmu, dmv );
    lk.DrawMotionArrows2( dmu2, dmv2 );

    cv::Mat remapC, remap;
    remapC = lk.Remap2to1( img2C, u, v );
    
    cvtColor( remapC, remap, CV_BGR2GRAY );
    cv::Mat diffRemap;
    diffRemap = lk.GetDiff( remap, img1 );
    cv::Mat diff21;
    diff21 = lk.GetDiff( img2, img1 );

    /// Write images
    imwrite("proj5-2-2-U.png", dmu2 );    
    imwrite("proj5-2-2-V.png", dmv2 );    
    imwrite("proj5-2-2-remap.png", remapC );
    imwrite("proj5-2-2-diffRemap.png", diffRemap );
    imwrite("proj5-2-2-diff21.png", diff21 );

    // Show results
    cv::namedWindow("U2", CV_WINDOW_NORMAL );
    cv::namedWindow("V2", CV_WINDOW_NORMAL );
    cv::namedWindow("Remap 2 to 1", CV_WINDOW_NORMAL );
    cv::namedWindow("Diff Image", CV_WINDOW_NORMAL );
    cv::namedWindow("21Diff", CV_WINDOW_NORMAL );

    imshow( "U2", dmu2 );
    imshow( "V2", dmv2 );

    imshow( "Remap 2 to 1", remapC );
    imshow( "Diff Image", diffRemap );
    imshow(  "21Diff", diff21 );
    cv::waitKey(0);  

}

