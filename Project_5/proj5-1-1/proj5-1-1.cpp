/**
 * @file proj5-1-1.cpp
 * @brief Reduce (Gaussian Pyramid)
 * @author A. Huaman
 */

#include <vector>
#include "Pyramid.h"

std::vector<cv::Mat> GP;

/**
 * @function main
 */
int main( int argc, char* argv[] ) {

    // Read image
    if( argc < 2 )
    { printf("You moron! I need an image \n"); return -1; }

    // Load image
    cv::Mat img = cv::imread( argv[1], -1 );

    // Create a Pyramid object
    Pyramid pyr;

    // Apply REDUCE 4 times to build the Gaussian Pyramid
    int wsize = 5;
   
    GP.resize(4);
    GP[0] = img.clone();
    GP[1] = pyr.Reduce( GP[0], 1*wsize );
    GP[2] = pyr.Reduce( GP[1], 2*wsize );
    GP[3] = pyr.Reduce( GP[2], 4*wsize );     


    // Write images
    imwrite("proj5-1-1-G0.png", GP[0]);    
    imwrite("proj5-1-1-G1.png", GP[1]);    
    imwrite("proj5-1-1-G2.png", GP[2]);    
    imwrite("proj5-1-1-G3.png", GP[3]);  

    // Show results
    cv::namedWindow("G0", CV_WINDOW_NORMAL );
    cv::namedWindow("G1", CV_WINDOW_NORMAL );
    cv::namedWindow("G2", CV_WINDOW_NORMAL );
    cv::namedWindow("G3", CV_WINDOW_NORMAL );     

    imshow( "G0", GP[0] );
    imshow( "G1", GP[1] );
    imshow( "G2", GP[2] );
    imshow( "G3", GP[3] );

    cv::waitKey(0);  
}

