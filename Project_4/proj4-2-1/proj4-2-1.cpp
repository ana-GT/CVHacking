/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"
#include "Matching.h"

char* windowOrient1 = "Orientation 1";
char* windowOrient2 = "Orientation 2";

cv::Mat img_orientation;
cv::Mat img1; cv::Mat img2;
cv::Mat img_color1; cv::Mat img_color2;

std::vector<cv::Point> keyPoints1;
std::vector<cv::Point> keyPoints2;

int thresh = 160;
int radius = 14;

Gradient g1;
Gradient g2;

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
   img1 = cv::imread( argv[1], 0 );
   img2 = cv::imread( argv[2], 0 );
   img_color1 = cv::imread( argv[1], 1 );
   img_color2 = cv::imread( argv[2], 1 );
   cv::Mat smooth_img1;
   cv::Mat smooth_img2;

   // Gauss filter
   cv::GaussianBlur( img1, smooth_img1, cv::Size(3,3),0,0 );
   cv::GaussianBlur( img2, smooth_img2, cv::Size(3,3),0,0 );

   g1.Init( smooth_img1 );
   g2.Init( smooth_img2 );

   // Get gradient here
   g1.GetGradients();
   g2.GetGradients();
   g1.GetHarris( 0.106 );
   g2.GetHarris( 0.106 );

   g1.GetImageHarris();
   g2.GetImageHarris();

   cv::Mat gx1 = g1.mGradientX; cv::Mat gy1 = g1.mGradientY;
   cv::Mat gx2 = g2.mGradientX; cv::Mat gy2 = g2.mGradientY;

   keyPoints1 = g1.GetHarrisPoints( 125, 10 );
   keyPoints2 = g2.GetHarrisPoints( 111,13 );

   m.Init_Pair1( img_color1, smooth_img1,  keyPoints1, gx1, gy1 );
   m.Init_Pair2( img_color2, smooth_img2,  keyPoints2, gx2, gy2 );

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

