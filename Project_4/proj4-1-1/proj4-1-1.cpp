/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"

char* windowGx = "Gradient X";
char* windowGy = "Gradient Y";
char* windowDebug = "Debug";

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

   Gradient g( smooth_img );

   // Get gradient here
   cv::Mat img_gx = g.GetImageGradientX();
   cv::Mat img_gy = g.GetImageGradientY();
   cv::Mat img_gxy = g.GetImageGradientXY();

   // Save results
   imwrite( "proj4-1-1-gx.png", img_gx );
   imwrite( "proj4-1-1-gy.png", img_gy );
   imwrite( "proj4-1-1.png", img_gxy );
 
   // Show it to me
   cv::namedWindow( windowGx, CV_WINDOW_NORMAL );
   cv::namedWindow( windowGy, CV_WINDOW_NORMAL );

   cv::imshow( windowGx, img_gx );
   cv::imshow( windowGy, img_gy );


   // Debug
   cv::Mat grad;
   addWeighted( img_gx, 0.5, img_gy, 0.5, 0, grad );
   cv::namedWindow( windowDebug, CV_WINDOW_NORMAL );  
   cv::imshow( windowDebug, grad);

   cv::waitKey(0);

   return 0;
}
