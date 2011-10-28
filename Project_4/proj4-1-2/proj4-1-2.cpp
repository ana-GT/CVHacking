/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"

char* windowHarris = "Harris image";

cv::Mat img_harris;
int thresh = 80;
int maxThresh = 100;
Gradient g;

void OnThresh( int, void* );

/**
 * @function main
 */
int main( int argc, char* argv[] )
{
   if( argc < 2 )
   { printf("You moron! I need an image \n"); return -1; }

   // Load grayscale image
   cv::Mat img = cv::imread( argv[1], 0 );
   cv::Mat smooth_img;

   // Gauss filter
   cv::GaussianBlur( img, smooth_img, cv::Size(3,3),0,0 );

   g.Init( smooth_img );

   // Get gradient here
   g.GetGradients();

   cv::namedWindow( windowHarris, CV_WINDOW_NORMAL );
   cv::createTrackbar("Thresh", windowHarris, &thresh, maxThresh, OnThresh );
   OnThresh(0,0);

   cv::waitKey(0);

   return 0;
}


/**
 * @function OnThresh
 */
void OnThresh( int, void* )
{
   double mThresh; /** between 0.04 - 0.15 */
   mThresh = 0.04 + (double)thresh*(0.15 - 0.04)/100.0;
   printf("--> Thresh: %.3f \n", mThresh );

   g.GetHarris( mThresh );
   img_harris = g.GetImageHarris();

   // Save results
   imwrite( "proj4-1-2-harris.png", img_harris );

   cv::imshow( windowHarris, img_harris );
}
