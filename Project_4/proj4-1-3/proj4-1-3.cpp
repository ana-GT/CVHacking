/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"

char* windowCorners = "Detected corners";
cv::Mat img_corners;
cv::Mat img;
cv::Mat imgColor;
cv::Mat img_harris;
int thresh = 160;
int radius = 14;
int maxRadius = 20;
int maxThresh = 255;

Gradient g;
cv::RNG rng(12345);

void OnThresh( int, void* );

/**
 * @function main
 */
int main( int argc, char* argv[] )
{
   if( argc < 2 )
   { printf("You moron! I need an image \n"); return -1; }

   // Load grayscale image
   img = cv::imread( argv[1], 0 );
   imgColor = cv::imread( argv[1], 1 );
   cv::Mat smooth_img;

   // Gauss filter
   cv::GaussianBlur( img, smooth_img, cv::Size(3,3),0,0 );

   g.Init( smooth_img );

   // Get gradient here
   g.GetGradients();
   g.GetHarris( 0.106 );
   img_harris = g.GetImageHarris();

   cv::namedWindow( windowCorners, CV_WINDOW_NORMAL );
   cv::createTrackbar("Thresh", windowCorners, &thresh, maxThresh, OnThresh );
   cv::createTrackbar("Radius", windowCorners, &radius, maxRadius, OnThresh );
   OnThresh(0, 0);

   cv::waitKey(0);

   return 0;

}


/**
 * @function OnThresh
 */
void OnThresh( int, void* )
{
   std::vector<cv::Point> hp = g.GetHarrisPoints( thresh, radius );
   printf("--> Number of corners: %d \n", hp.size() );

   img_corners = imgColor.clone();

   for( size_t i = 0; i < hp.size(); i++ )
   {
      circle( img_corners, hp[i], 4, cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255)), -1, 8, 0 );
   }

   // Save results
   imwrite( "proj4-1-3-corners.png", img_corners );

   cv::imshow( windowCorners, img_corners );
}

