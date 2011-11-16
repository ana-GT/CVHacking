/**
 * @file Gradient.cpp
 * @date 2011-10-22
 * @author A. Huaman
 */

#include "Gradient.h"
#include "Matching.h"
#include "Ransac.h"

char* windowMatches = "Matches";
char* windowConsensus = "Consensus Set";
char* windowStitched = "Stitched image";
cv::Mat stitched;

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

   printf("Size matches: %d \n", m.mMatches.size() );
   printf("Size Good matches: %d \n", m.mGoodMatches.size() );
  
 
   cv::namedWindow( windowMatches, CV_WINDOW_NORMAL );
   imshow( windowMatches, m.DrawMatches() );

   m.Surfie();
   Ransac r;
   printf("N: %d \n", r.mSimilarity_N );
   r.RansacSimilarity( m.mKeyPoints1, 
												m.mKeyPoints2,    
                        m.mMatches );

   cv::Mat consensusImage = r.drawConsensusSet( img1, img2 );

   cv::namedWindow( windowConsensus, CV_WINDOW_NORMAL );
   imshow( windowConsensus, consensusImage );

   imwrite("proj4-3-2.png", consensusImage );

   /// Showing results

   cv::Mat M = cv::Mat( 2, 3, CV_32FC1 );
   M.at<float>(0,0) = 1.138873; M.at<float>(0,1) =0.974904; M.at<float>(0,2) = -109.045906;
   M.at<float>(1,0) = -0.974904; M.at<float>(1,1) = 1.138873; M.at<float>(1,2) = 264.759216;

   cv::Mat dst1 = cv::Mat::zeros( img1.rows, img1.cols, img1.type() );
   cv::warpAffine( img1, dst1, M, img1.size(), cv::INTER_LINEAR );
   cv::addWeighted( dst1, 0.5, img2, 0.5, 0.0, stitched );
   
   cv::namedWindow( windowStitched, CV_WINDOW_NORMAL );
   imshow( windowStitched, stitched );

  
   cv::waitKey(0);

   return 0;

}

