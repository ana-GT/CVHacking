/**
 * @file proj5-1-2.cpp
 * @brief Reduce (Gaussian Pyramid)
 * @author A. Huaman
 */

#include <vector>
#include "Pyramid.h"

std::vector<cv::Mat> GP;
std::vector<cv::Mat> LP;

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
   
    GP.resize(5);
    LP.resize(4);
    GP[0] = img.clone();
    GP[1] = pyr.Reduce( GP[0] );
    GP[2] = pyr.Reduce( GP[1] );
    GP[3] = pyr.Reduce( GP[2] ); 

    cv::Mat GP0, GP1, GP2, GP3;

    cvtColor( GP[0], GP0, CV_BGR2GRAY );
    cvtColor( GP[1], GP1, CV_BGR2GRAY );
    cvtColor( GP[2], GP2, CV_BGR2GRAY );
    cvtColor( GP[3], GP3, CV_BGR2GRAY );

    cv::Mat EP1, EP2, EP3;

    cvtColor( pyr.Expand( GP[1] ), EP1, CV_BGR2GRAY );
    cvtColor( pyr.Expand( GP[2] ), EP2, CV_BGR2GRAY );
    cvtColor( pyr.Expand( GP[3] ), EP3, CV_BGR2GRAY );


    LP[0] = GP0 - EP1;
    LP[1] = GP1 - EP2;
    LP[2] = GP2 - EP3;
    LP[3] = GP3;

    // Write images
    imwrite("proj5-1-2-LP0.png", LP[0] );    
    imwrite("proj5-1-2-LP1.png", LP[1] );    
    imwrite("proj5-1-2-LP2.png", LP[2] );    
    imwrite("proj5-1-2-LP3.png", LP[3] );    

    //imwrite("proj5-1-2-E0.png", pyr.Expand( GP[1] ));
    //imwrite("proj5-1-2-E1.png", pyr.Expand( GP[2] ));
    //imwrite("proj5-1-2-E2.png", pyr.Expand( GP[3] ));


    // Show results
    cv::namedWindow("LP0", CV_WINDOW_NORMAL );
    cv::namedWindow("LP1", CV_WINDOW_NORMAL );
    cv::namedWindow("LP2", CV_WINDOW_NORMAL );
    cv::namedWindow("LP3", CV_WINDOW_NORMAL );

    imshow( "LP0", LP[0] );
    imshow( "LP1", LP[1] );
    imshow( "LP2", LP[2] );
    imshow( "LP3", LP[3] );

    cv::waitKey(0);  
}

