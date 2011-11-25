
#include "Pyramid.h"


/**
 * @function Pyramid
 * @brief Destructor
 */
Pyramid::Pyramid() {

mTabW[0][0] = 1.0/16.0; mTabW[0][1] = 4.0/16.0; mTabW[0][2] = 6.0/16.0; mTabW[0][3] = 4.0/16.0;mTabW[0][4] = 1.0/16.0; 
mTabW[1][0] = 4.0/16.0; mTabW[1][1] = 16.0/16.0; mTabW[1][2] = 24.0/16.0; mTabW[1][3] = 16.0/16.0;mTabW[1][4] = 4.0/16.0; 
mTabW[2][0] = 6.0/16.0; mTabW[2][1] = 24.0/16.0; mTabW[2][2] = 36.0/16.0; mTabW[2][3] = 24.0/16.0;mTabW[2][4] = 6.0/16.0; 
mTabW[3][0] = 4.0/16.0; mTabW[3][1] = 16.0/16.0; mTabW[3][2] = 24.0/16.0; mTabW[3][3] = 16.0/16.0;mTabW[3][4] = 4.0/16.0; 
mTabW[4][0] = 1.0/16.0; mTabW[4][1] = 4.0/16.0; mTabW[4][2] = 6.0/16.0; mTabW[4][3] = 4.0/16.0;mTabW[4][4] = 1.0/16.0; 

mWSize = 5;
mWSize2 = 2;

}

/**
 * @function Pyramid
 * @brief Destructor
 */
Pyramid::~Pyramid() {

}

/**
 * @function Reduce
 */
cv::Mat Pyramid::Reduce( const cv::Mat &_input ) {

    // Check that input image is multiple of 2
    if( _input.rows % 2 != 0 || _input.cols % 2 != 0 ) {
        printf("--(x) Image cols and rows should be multiple of 2, idiot! Exiting now! \n");
        return cv::Mat::zeros( _input.rows, _input.cols, _input.type() );        
    }

    // Set some values
    int rows = _input.rows;
    int cols = _input.cols;
    int rows2 = _input.rows / 2;
    int cols2 = _input.cols / 2;


    // Create reduced Matrix
    float *reducedFloat;
    reducedFloat = new float[rows2*cols2];
    cv::Mat reduced = cv::Mat::zeros( rows2, cols2, CV_8UC3 );

    // Apply for each channel 

    for( int k = 0; k < 3; k++ ) {

      float sum;
      float g;
      int ind = 0;

      for( int j = 0; j < rows2; j++ ) {
        for( int i = 0; i < cols2; i++ ) {
          sum = 0;      
          /** Sum weighted */   
          for( int m = -mWSize2; m <= mWSize2; m++ ) {
            for( int n = -mWSize2; n <= mWSize2; n++ ) {
              if( 2*j + m < 0 || 2*j + m >= rows || 2*i + n < 0 || 2*i + n >= cols )
                  g = 0;
              else
                  g= (float) _input.at<cv::Vec3b>(2*j + m, 2*i + n )[k]; 
              sum += mTabW[m + mWSize2][n + mWSize2]*g;
            }     
          } 
          /** End sum weighted */
          reducedFloat[ind] = sum;
          ind++;
        }
      }

      // Go to integer values
     float minG = *std::min_element( reducedFloat, reducedFloat + rows2*cols2 );
     float maxG = *std::max_element( reducedFloat, reducedFloat + rows2*cols2 );

     printf("--min Reduce: %.4f channel: %d \n", minG, k );
     printf("--max Reduce: %.4f channel: %d \n", maxG, k );

     ind = 0;
     for( unsigned int j = 0; j < rows2; j++ )
     {  for( unsigned int i = 0; i < cols2; i++ )
        {
           uchar d = (uchar) 255.0*( reducedFloat[ind] - minG )/( maxG - minG );
           reduced.at<cv::Vec3b>(j,i)[k] = d; 
           ind++; 
        }
     }

   } // end for channels

  return reduced; 
}

/**
 * @function Expand
 */
cv::Mat Pyramid::Expand( const cv::Mat &_input ) {

    // Set some values
    int rows = _input.rows;
    int cols = _input.cols;
    int rows2 = _input.rows*2;
    int cols2 = _input.cols*2;

    // Create expanded Matrix
    float *expandedFloat;
    expandedFloat = new float[rows2*cols2];
    cv::Mat expanded = cv::Mat::zeros( rows2, cols2, CV_8UC3 );

    // Apply for each channel 
    for( int k = 0; k < 3; k++ ) {
      float sum;
      float g;
      int ind = 0;
      for( int j = 0; j < rows2; j++ ) {
        for( int i = 0; i < cols2; i++ ) {
          sum = 0;    
          /** Sum weighted */       
          for( int m = -mWSize2; m <= mWSize2; m++ ) {
            for( int n = -mWSize2; n <= mWSize2; n++ ) {
              if( (j - m)%2 != 0 || (i - n)%2 != 0 )
                { g = 0; } 
              else if( (j - m)/2 < 0 || (j - m)/2 >= rows || (i - n)/2 < 0 || (i - n)/2 >= cols )
                { g = 0; } 
              else
                { g= (float) _input.at<cv::Vec3b>( (j - m)/2, (i - n)/2 )[k]; }

              sum += (mTabW[m + mWSize2][n + mWSize2])*g;
            }     
          } 
          /** End sum weighted */
          expandedFloat[ind] = sum;
          ind++;
        }
      }

      // Go to integer values
     float minG = *std::min_element( expandedFloat, expandedFloat + rows2*cols2 );
     float maxG = *std::max_element( expandedFloat, expandedFloat + rows2*cols2 );

     printf("--min Expanded: %.4f channel: %d \n", minG, k );
     printf("--max Expanded: %.4f channel: %d \n", maxG, k );

     ind = 0;
     for( unsigned int j = 0; j < rows2; j++ )
     {  for( unsigned int i = 0; i < cols2; i++ )
        {
           uchar d = (uchar) 255.0*( expandedFloat[ind] - minG )/( maxG - minG );
           expanded.at<cv::Vec3b>(j,i)[k] = d; 
           ind++; 
        }
     }

   } // end for channels

  return expanded;
}
