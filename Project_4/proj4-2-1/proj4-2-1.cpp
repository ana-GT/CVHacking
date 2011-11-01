/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"
#include "Matching.h"

char* windowOrient1 = "Orientation 1";
char* windowOrient2 = "Orientation 2";
cv::Mat img1;
cv::Mat img2;

Matching m;
cv::RNG rng(12345);

/**
 * @function main
 */
int main( int argc, char* argv[] )
{
   if( argc < 3 )
   { printf("You moron! I need two image for SIFT at the very least \n"); return -1; }

   // Load grayscale image
   img1 = cv::imread( argv[1], 1 );
   img2 = cv::imread( argv[2], 1 );

   m.Init_Pair1( img1, 0.05, 218, 9 );
   m.Init_Pair2( img2, 0.05, 218, 9 );


   cv::Mat handy1 = m.DrawOrient1Handy();
   cv::Mat handy2 = m.DrawOrient2Handy();

   imwrite( "proj4-2-1-orientA2.png", handy1 );
   imwrite( "proj4-2-1-orientB2.png", handy2 );

   cv::namedWindow( "Handy 1", CV_WINDOW_NORMAL );
   cv::namedWindow( "Handy 2", CV_WINDOW_NORMAL );


   imshow("Handy 1", handy1 );
   imshow("Handy 2", handy2 );


   cv::Mat orient1 = m.DrawOrient1();
   cv::Mat orient2 = m.DrawOrient2();

   imwrite( "proj4-2-1-orientA.png", orient1 );
   imwrite( "proj4-2-1-orientB.png", orient2 );

   cv::namedWindow( windowOrient1, CV_WINDOW_NORMAL );
   cv::namedWindow( windowOrient2, CV_WINDOW_NORMAL );

   imshow( windowOrient1, orient1 );
   imshow( windowOrient2, orient2 );

   
   cv::waitKey(0);

   return 0;

}

