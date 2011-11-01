
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
void Matching::Init_Pair1( const cv::Mat &_img1, 
                           double _coeff1, 
                           int _thresh1, 
                           int _radius1 ) {

   mColor1 = _img1.clone();
   mHarrisCoeff1 = _coeff1;
   mThresh1 = _thresh1;
   mRadius1 = _radius1;

   mRows1 = _img1.rows;
   mCols1 = _img1.cols;


   /// Save gray version
   cvtColor( mColor1, mPair1, CV_BGR2GRAY );

   // Gauss filter
   cv::GaussianBlur( mPair1, mPair1, cv::Size(3,3),0,0 );

   mG1.Init( mPair1 );

   /// Get gradient here
   mG1.GetGradients();
   mG1.GetHarris( mHarrisCoeff1 );

   mG1.GetImageHarris();

   mPoints1 = mG1.GetHarrisPoints( mThresh1, mRadius1 );
   mKeyPoints1Num = mPoints1.size();

   /// Create KeyPoints
   mKeyPoints1.resize(mKeyPoints1Num);

   for( unsigned int i = 0; i < mKeyPoints1Num; i++ ) {

      float ang = (float)atan2( (float) mG1.mGradientY.at<short>( (int)mPoints1[i].y, (int) mPoints1[i].x), 
																 (float) mG1.mGradientX.at<short>( (int)mPoints1[i].y, (int)mPoints1[i].x) );
      cv::KeyPoint kp1 = cv::KeyPoint( mPoints1[i], 
                                       16,
                                       (float)ang*180.0/3.1416, 0, 0 );
      mKeyPoints1[i] = kp1;
   }

   /// Create Angle matrix
   mAngles1 = cv::Mat( mRows1, mCols1, CV_32FC1 );

   for( unsigned int j = 0; j < mRows1; j++ )
   {  for( unsigned int i = 0; i < mCols1; i++ )
      {
         mAngles1.at<float>(j,i) = (float)atan2( (float) mG1.mGradientY.at<short>(j,i), 
																		             (float) mG1.mGradientX.at<short>(j,i) );
      }
   }

}
 
/**
 * @function InitPair2
 */
void Matching::Init_Pair2( const cv::Mat &_img2, 
												   double _coeff2, 
                           int _thresh2, 
                           int _radius2 ) {

   mColor2 = _img2.clone();
   mHarrisCoeff2 = _coeff2;
   mThresh2 = _thresh2;
   mRadius2 = _radius2;

   mRows2 = _img2.rows;
   mCols2 = _img2.cols;


   /// Save gray version
   cvtColor( mColor2, mPair2, CV_BGR2GRAY );

   // Gauss filter
   cv::GaussianBlur( mPair2, mPair2, cv::Size(3,3),0,0 );

   mG2.Init( mPair2 );

   /// Get gradient here
   mG2.GetGradients();
   mG2.GetHarris( mHarrisCoeff2 );

   mG2.GetImageHarris();

   mPoints2 = mG2.GetHarrisPoints( mThresh2, mRadius2 );
   mKeyPoints2Num = mPoints2.size();

   /// Create KeyPoints
   mKeyPoints2.resize(mKeyPoints2Num);

   for( unsigned int i = 0; i < mKeyPoints2Num; i++ ) {

      float ang = (float)atan2( (float) mG2.mGradientY.at<short>(mPoints2[i].y, mPoints2[i].x), 
																 (float) mG2.mGradientX.at<short>(mPoints2[i].y, mPoints2[i].x) );
      cv::KeyPoint kp2 = cv::KeyPoint( mPoints2[i], 
                                       16,
                                       ang*180.0/3.1416, 0, 0 );
      mKeyPoints2[i] = kp2;
   }

   /// Create Angle matrix

   mAngles2 = cv::Mat( mRows2, mCols2, CV_32FC1 );

   for( unsigned int j = 0; j < mRows2; j++ )
   {  for( unsigned int i = 0; i < mCols2; i++ )
      {
         mAngles2.at<float>(j,i) = (float)atan2( (float) mG2.mGradientY.at<short>(j,i), 
																		             (float) mG2.mGradientX.at<short>(j,i) );
      }
   }
}

/**
 * @function DrawOrient1
 */
cv::Mat Matching::DrawOrient1() {

  cv::Mat drawOrient1 = mColor1.clone();

  cv::drawKeypoints( mColor1, mKeyPoints1, drawOrient1, cv::Scalar::all(-1), 
                     cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS ); 

  return drawOrient1;
}


/**
 * @function DrawOrient1Handy
 */
cv::Mat Matching::DrawOrient1Handy() {

  cv::Mat drawOrient1 = mColor1.clone();

  float r = 16;

  for( unsigned int i = 0; i < mKeyPoints1Num; i++ ) {
    
    float theta = mAngles1.at<float>( (int) mPoints1[i].y, (int) mPoints1[i].x );
    cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));

    /// Draw point
    circle( drawOrient1, mPoints1[i], 4, color, -1, 8, 0 );
    /// Draw orient -- CHANGED ORIENTATION OF Y TO MATCH THE KEYPOINTS DRAWING
    line( drawOrient1, 
          mPoints1[i], 
          cv::Point2f(mPoints1[i].x + r*cos(theta), mPoints1[i].y - r*sin(theta)), 
          color, 2, 8 );
   }

  return drawOrient1;

}

/**
 * @function DrawOrient
 */
cv::Mat Matching::DrawOrient2() {

  cv::Mat drawOrient2 = mColor2.clone();

  cv::drawKeypoints( mColor2, mKeyPoints2, drawOrient2, cv::Scalar::all(-1), 
                     cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS ); 

  return drawOrient2;

}


/**
 * @function DrawOrientHandy
 */
cv::Mat Matching::DrawOrient2Handy() {

  cv::Mat drawOrient2 = mColor2.clone();

  float r = 16;

  for( unsigned int i = 0; i < mKeyPoints2Num; i++ ) {
    
    float theta = mAngles2.at<float>( mPoints2[i].y, mPoints2[i].x );
    cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));

    /// Draw point
    circle( drawOrient2, mPoints2[i], 4, color, -1, 8, 0 );
    /// Draw orient -- CHANGED ORIENTATION OF Y TO MATCH THE KEYPOINTS DRAWING
    line( drawOrient2, 
          mPoints2[i], 
          cv::Point(mPoints2[i].x + r*cos(theta), mPoints2[i].y - r*sin(theta)), 
          color, 2, 8 );
   }

  return drawOrient2;

}
