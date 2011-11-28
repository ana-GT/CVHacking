/**
 * @file proj5-2-1.cpp
 * @brief Motion Field
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
    cv::Mat img1 = cv::imread( argv[1], 0 );
    cv::Mat img2 = cv::imread( argv[2], 0 );

    /// Create flow image
    cv::Mat u, v;
    LK lk;
    lk.OpticFlowEstimation1( img1, img2, u, v );
    cv::Mat dmu, dmv;
    lk.DrawMotionArrows( dmu, dmv );

    cv::Mat remap;
    remap = lk.Remap2to1( img2, u, v );

    cv::Mat diffRemap;
    diffRemap = lk.GetDiff( remap, img1 );
    cv::Mat diffOriginal;
    diffOriginal = lk.GetDiff( img2, img1 );


    /// Write images
    imwrite("proj5-2-1-U.png", dmu );    
    imwrite("proj5-2-1-V.png", dmv );    
    imwrite("proj5-2-1-remap.png", remap );

    // Show results
    cv::namedWindow("U", CV_WINDOW_NORMAL );
    cv::namedWindow("V", CV_WINDOW_NORMAL );
    cv::namedWindow("Original Diff", CV_WINDOW_NORMAL );
    cv::namedWindow("Remapping Diff", CV_WINDOW_NORMAL );
    cv::namedWindow("Remap", CV_WINDOW_NORMAL );

    imshow( "U", dmu );
    imshow( "V", dmv );
    imshow("Original Diff", diffOriginal);
    imshow("Remapping Diff", diffRemap);
    imshow("Remap", remap );
    cv::waitKey(0);  
}

