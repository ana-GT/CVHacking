
#include "Pyramid.h"


/**
 * @function Pyramid
 * @brief Destructor
 */
Pyramid::Pyramid() {

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
cv::Mat Pyramid::Reduce( const cv::Mat &_input, int _windowSize ) {

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
    int wsize = _windowSize;
    int wsize2 = wsize / 2;

    // Pad it by the size of the window
    cv::Mat padded = cv::Mat( rows + 2*wsize2, cols + 2*wsize2, CV_8UC3 );
   
    cv::copyMakeBorder( _input, padded, 
                        wsize2, wsize2, 
                        wsize2, wsize2, 
                        cv::BORDER_REPLICATE, cv::Scalar::all(0) );  

    // Create the filter to apply
    float tau = 0.84089;
    float window[wsize][wsize];
    for( int j = -wsize2; j <= wsize2; j++ ) {
      for( int i = -wsize2; i <= wsize2; i++ ) {
          window[j+wsize2][i+wsize2] = (1/(2*3.1416*tau*tau))*pow(2.718281828, -(j*j+i*i)/(2*tau*tau));
      }
    } 


    // Create reduced Matrix
    float *reducedFloat;
    reducedFloat = new float[rows2*cols2];
    cv::Mat reduced = cv::Mat::zeros( rows2, cols2, CV_8UC3 );

    // Apply for each channel 
    for( int k = 0; k < 3; k++ ) {

      float sum;
      int ind = 0;
      for( unsigned j = 0; j < rows2; j++ ) {
        for( unsigned i = 0; i < cols2; i++ ) {
          sum = 0;         
          for( int m = -wsize2; m <= wsize2; m++ ) {
            for( int n = -wsize2; n <= wsize2; n++ ) {
              float g= (float) padded.at<cv::Vec3b>(2*j + m + wsize2, 2*i + n + wsize2)[k]; 
              sum += window[m+wsize2][n+wsize2]*g;
            }     
          } 
          reducedFloat[ind] = sum;
          ind++;
        }
      }

      // Go to integer values
     float minG = *std::min_element( reducedFloat, reducedFloat + rows2*cols2 );
     float maxG = *std::max_element( reducedFloat, reducedFloat + rows2*cols2 );

     printf("--minG: %.4f channel: %d \n", minG, k );
     printf("--maxG: %.4f channel: %d \n", maxG, k );

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
cv::Mat Pyramid::Expand( const cv::Mat &_input, int _windowSize ) {

    cv::Mat expanded = _input.clone();
    return expanded;
}
