
#include "Matching.h"


/**
 * @function Matching
 * @brief Constructor
 */
Matching::Matching() {

  rng(12345);

}

/**
 * @function Matching
 * @brief Destructor
 */
Matching::~Matching() {
}

/**
 * @function InitPair1
 */
void Matching::Init_Pair1( const cv::Mat &_img1_color, 
                           const cv::Mat &_img1, 
                           std::vector<cv::Point> &_keypoints1, 
                           const cv::Mat &_gx1, 
                           const cv::Mat &_gy1 ) {

   mRows1 = _img1.rows;
   mCols1 = _img1.cols;

   mColor1 = _img1_color;
   mPair1 = _img1;
   mKeyPoints1Num = _keypoints1.size();

   mKeyPoints1.resize(0);

   for( unsigned int i = 0; i < mKeyPoints1Num; i++ ) {
     mKeyPoints1.push_back( _keypoints1[i] );
   }


   mAngles1 = cv::Mat( mRows1, mCols1, CV_32FC1 );

   for( unsigned int j = 0; j < mRows1; j++ )
   {  for( unsigned int i = 0; i < mCols1; i++ )
      {
         mAngles1.at<float>(j,i) = (float)atan2( _gy1.at<short>(j,i), _gx1.at<short>(j,i) );
      }
   }

}
 
/**
 * @function InitPair2
 */
void Matching::Init_Pair2( const cv::Mat &_img2_color,
                           const cv::Mat &_img2, 
                           std::vector<cv::Point> &_keypoints2, 
                           const cv::Mat &_gx2, 
                           const cv::Mat &_gy2 ) {

   mRows2 = _img2.rows;
   mCols2 = _img2.cols;

   mColor2 = _img2_color;
   mPair2 = _img2;
   mKeyPoints2Num = _keypoints2.size();

   mKeyPoints2.resize(0);
   for( unsigned int i = 0; i < mKeyPoints2Num; i++ ) {
     mKeyPoints2.push_back( _keypoints2[i] );
   }


   mAngles2 = cv::Mat( mRows2, mCols2, CV_32FC1 );

   for( unsigned int j = 0; j < mRows2; j++ )
   {  for( unsigned int i = 0; i < mCols2; i++ )
      {
         mAngles2.at<float>(j,i) = (float) atan2( _gy2.at<short>(j,i), _gx2.at<short>(j,i) );
      }
   }
}

/**
 * @function DrawOrient
 */
cv::Mat Matching::DrawOrient1() {

  cv::Mat drawOrient1 = mColor1;
  float r = 20;

  for( unsigned int i = 0; i < mKeyPoints1Num; i++ ) {
    
    float theta = mAngles1.at<float>( mKeyPoints1[i].y, mKeyPoints1[i].x );
    cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));

    /// Draw point
    circle( drawOrient1, mKeyPoints1[i], 2, color, -1, 8, 0 );
    /// Draw orient
    line( drawOrient1, 
          mKeyPoints1[i], 
          cv::Point2f( mKeyPoints1[i].x + r*cos(theta), mKeyPoints1[i].y + r*sin(theta)), 
          color, 4, 8 );
   }

  return drawOrient1;

}

/**
 * @function DrawOrient
 */
cv::Mat Matching::DrawOrient2() {


  cv::Mat drawOrient2 = mColor2;
  float r = 20;

  for( unsigned int i = 0; i < mKeyPoints2Num; i++ ) {
    
    float theta = mAngles2.at<float>( mKeyPoints2[i].y, mKeyPoints2[i].x );
    cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));

    /// Draw point
    circle( drawOrient2, mKeyPoints2[i], 2, color, -1, 8, 0 );
    /// Draw orient
    line( drawOrient2, 
          mKeyPoints2[i], 
          cv::Point(mKeyPoints2[i].x + r*cos(theta), mKeyPoints2[i].y + r*sin(theta)), 
          color, 4, 8 );
   }

  return drawOrient2;

}
