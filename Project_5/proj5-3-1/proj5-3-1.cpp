/**
 * @file proj5-3-1
 * @brief Hierarchical guy
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
#include "Pyramid.h"

/**
 * @function main
 */
int main( int argc, char* argv[] ) {

    /// Read image
    if( argc < 3 )
    { printf("You moron! I need at least two images \n"); return -1; }

    /// Load images as they are
    cv::Mat L = cv::imread( argv[1], -1 );
    cv::Mat R = cv::imread( argv[2], -1 );
  
    // Have ready my Pyramid  and LK object
    Pyramid pyr;
    LK lk;

    // Hierarchical LK
    int n = 4; // Max level 
    int k;

    // Get the required pyramid levels
    std::vector<cv::Mat> PL(n+1);
    std::vector<cv::Mat> PR(n+1);

    PL[0] = L.clone();
    PR[0] = R.clone();

    for( int i = 1; i <= n; i++ ) {
      PL[i] = pyr.Reduce( PL[i-1] );
      PR[i] = pyr.Reduce( PR[i-1] );
    }

    /// Let's start the real thing
    cv::Mat Lk, Rk;
    cv::Mat U, V;
    cv::Mat Dx, Dy; 
    cv::Mat Wk;
    cv::Mat Wkg, Rkg, Lkg;

    /// 1. Initialize k = n
    k = n;

    while( k >= 0 ) {

      /// 2. Reduce both images to level k
      Lk = PL[k]; 
      Rk = PR[k]; 

      /// 3. If k = n initialize U and K to zeros of the size of Lk 
      if( k == n ) {
          U = cv::Mat::zeros( Lk.size(), CV_32FC1 );
          V = cv::Mat::zeros( Lk.size(), CV_32FC1 );
      }
      /// Else expand the flow field and double it
      else {
          cv::Mat tempU, tempV;
          tempU = pyr.ExpandFloat(U);
          tempV = pyr.ExpandFloat(V);

          U = cv::Mat( tempU.size(), CV_32FC1 );
          V = cv::Mat( tempV.size(), CV_32FC1 );

          for( int j = 0; j < U.rows; j++ )
          { for( int i = 0; i < U.cols; i++ )
            {
               U.at<float>(j,i) = ( 2.0*tempU.at<float>(j,i) );
               V.at<float>(j,i) = ( 2.0*tempV.at<float>(j,i) );
            }
          }
      }

      /// 4. Warp Lk using U and V to form Wk
      Wk = lk.Remap2to1( Rk, U, V );
      char buf[30];
      sprintf( buf, "Wk%d.png", k);
      imwrite( buf, Wk );

      /// 5. Perform LK on Wk and Rk
      cvtColor( Wk, Wkg, CV_BGR2GRAY );
      cvtColor( Lk, Lkg, CV_BGR2GRAY );
      cvtColor( Rk, Rkg, CV_BGR2GRAY );

      //lk.OpticFlowEstimation1( Lkg, Wkg, Dx, Dy, 2.0 );
      //lk.OpticFlowEstimation3( Lkg, Wkg, Dx, Dy, 2.0 );
   
      /// 6. Add these to the original flow
      for( int j = 0; j < U.rows; j++ )
      { for( int i = 0; i < U.cols; i++ )
        {
           U.at<float>(j,i) = U.at<float>(j,i) + Dx.at<float>(j,i);
           V.at<float>(j,i) = V.at<float>(j,i) + Dy.at<float>(j,i);
        }
      }

      cv::Mat temp = lk.Remap2to1( Rk, U, V );
      sprintf( buf, "Wk%dend.png", k);
      imwrite( buf, temp );   

      k = k - 1;
    } // end of while

    cv::Mat mU, mV;
    lk.DrawMotionArrows3( U, V, mU, mV );


    cv::Mat remap;
    remap = lk.Remap2to1( R, U, V );


    cv::Mat remapg;
    cvtColor( remap, remapg, CV_BGR2GRAY );
    cv::Mat diffRemap;
    diffRemap = lk.GetDiff( remapg, L );

    cv::Mat diff21;
    diff21 = lk.GetDiff( L, R );

    /// Write images
    imwrite("proj5-3-1-U.png", mU );    
    imwrite("proj5-3-1-V.png", mV );    
    imwrite("proj5-3-1-remap.png", remap );
    imwrite("proj5-3-1-diffRemap.png", diffRemap );
    //imwrite("proj5-3-1-diff21.png", diff21 );

    // Show results
    cv::namedWindow("U", CV_WINDOW_NORMAL );
    cv::namedWindow("V", CV_WINDOW_NORMAL );
    cv::namedWindow("Remap 2 to 1", CV_WINDOW_NORMAL );
    cv::namedWindow("Diff Image", CV_WINDOW_NORMAL );
    cv::namedWindow("21Diff", CV_WINDOW_NORMAL );

    imshow( "U", mU );
    imshow( "V", mV );

    imshow( "Remap 2 to 1", remap );
    imshow( "Diff Image", diffRemap );
    imshow(  "21Diff", diff21 );


    cv::waitKey(0);
    return 0;

}

