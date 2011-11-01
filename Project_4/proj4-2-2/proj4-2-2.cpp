/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"
#include "Matching.h"

char* windowMatches = "Matches";
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

   m.Init_Pair1( img1, 0.05, 124, 6 );
   m.Init_Pair2( img2, 0.05, 117, 3 );

   m.buildSIFTDescriptor1();
   m.buildSIFTDescriptor2();

   cv::Mat matches = m.BruteMatcher();

   imwrite( "proj4-2-2-matches.png", m.DrawMatches() );

   cv::namedWindow( windowMatches, CV_WINDOW_NORMAL );
   imshow( windowMatches, matches );

   //m.Surfie();   

   cv::waitKey(0);

   return 0;

}

