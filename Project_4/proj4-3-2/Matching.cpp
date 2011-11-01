
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
 * @function buildSIFTDescriptor1
 */
void Matching::buildSIFTDescriptor1() {
 
    cv::SIFT sift( 1.5, true, false );
    sift( mPair1, cv::Mat(), mKeyPoints1, mDescriptor1, true );
}

/**
 * @function buildSIFTDescriptor2
 */
void Matching::buildSIFTDescriptor2() {

    cv::SIFT sift( 1.0, true, false );
    sift(  mPair2, cv::Mat(), mKeyPoints2, mDescriptor2, true );
}

/**
 * @function BruteMatcher
 */
cv::Mat Matching::BruteMatcher() {

  cv::BruteForceMatcher< cv::L2<float> > matcher;
  mMatches.resize(0);
  /*
  cv::FlannBasedMatcher matcher; // I like this one better but well, brute has to be used
  std::vector< cv::DMatch > matches;
  */
  matcher.match( mDescriptor1, mDescriptor2, mMatches );

  double max_dist = 0; double min_dist = 1000;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < mDescriptor1.rows; i++ )
  { double dist = mMatches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );
  
  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  mGoodMatches.resize(0);

  for( int i = 0; i < mDescriptor1.rows; i++ )
  { if( mMatches[i].distance < 1.5*min_dist )
    { mGoodMatches.push_back( mMatches[i]); }
  } 

  printf("Size of good matches: %d \n", mGoodMatches.size());
  
  return DrawGoodMatches();

}

/**
 * @function Surfie
 */
cv::Mat Matching::Surfie()
{
     //-- Step 2: Calculate descriptors (feature vectors)
  cv::SurfDescriptorExtractor extractor;

  cv::Mat descriptors_1, descriptors_2;

  extractor.compute( mPair1, mKeyPoints1, descriptors_1 );
  extractor.compute( mPair2, mKeyPoints2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  cv::FlannBasedMatcher matcher;
  std::vector< cv::DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );

  double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );
  
  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  std::vector< cv::DMatch > good_matches;

  for( int i = 0; i < descriptors_1.rows; i++ )
  { if( matches[i].distance < 10*min_dist )
    { good_matches.push_back( matches[i]); }
  }  

  //-- Draw only "good" matches
  cv::Mat img_matches;
  cv::drawMatches( mColor1, mKeyPoints1, mColor2, mKeyPoints2, 
               good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), 
               std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS ); 

  //-- Show detected matches
  imshow( "Great Matches", img_matches );

  return img_matches;
}

/**
 * @function DrawGoodMatches
 */
cv::Mat Matching::DrawGoodMatches() {

   mGoodMatchesImage = cv::Mat( mRows1, mCols1*2, CV_8UC3 );
   
   for( unsigned int j = 0; j < mRows1; j++ )
   {  for( unsigned int i = 0; i < mCols1; i++ )
      {  for( int n = 0; n < 3; n++ )
         {  mGoodMatchesImage.at<cv::Vec3b>(j,i)[n] = mColor1.at<cv::Vec3b>(j,i)[n];
            mGoodMatchesImage.at<cv::Vec3b>(j,i + mCols1 )[n] = mColor2.at<cv::Vec3b>(j,i)[n];
         }
      }
   }

   for( unsigned int i = 0; i < mGoodMatches.size(); i++ )
   {
       cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
       circle( mGoodMatchesImage, mPoints1[ mGoodMatches[i].queryIdx], 4, color, 2, 8, 0 );
       circle( mGoodMatchesImage, cv::Point2f( mPoints2[ mGoodMatches[i].trainIdx].x + mCols1, mPoints2[ mGoodMatches[i].trainIdx].y ), 4, color, 2, 8, 0 );
       line( mGoodMatchesImage, 
             mPoints1[ mGoodMatches[i].queryIdx], 
             cv::Point2f( mPoints2[ mGoodMatches[i].trainIdx].x + mCols1, mPoints2[ mGoodMatches[i].trainIdx].y ), 
             color, 1, CV_AA );
   }

   return mGoodMatchesImage;

}


/**
 * @function DrawMatches
 */
cv::Mat Matching::DrawMatches() {

   mMatchesImage = cv::Mat( mRows1, mCols1*2, CV_8UC3 );
   
   for( unsigned int j = 0; j < mRows1; j++ )
   {  for( unsigned int i = 0; i < mCols1; i++ )
      {  for( int n = 0; n < 3; n++ )
         {  mMatchesImage.at<cv::Vec3b>(j,i)[n] = mColor1.at<cv::Vec3b>(j,i)[n];
            mMatchesImage.at<cv::Vec3b>(j,i + mCols1 )[n] = mColor2.at<cv::Vec3b>(j,i)[n];
         }
      }
   }

   for( unsigned int i = 0; i < mMatches.size(); i++ )
   {
       cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
       circle( mMatchesImage, mPoints1[ mMatches[i].queryIdx], 4, color, 2, 8, 0 );
       circle( mMatchesImage, cv::Point2f( mPoints2[ mMatches[i].trainIdx].x + mCols1, mPoints2[ mMatches[i].trainIdx].y ), 4, color, 2, 8, 0 );
       line( mMatchesImage, 
             mPoints1[ mMatches[i].queryIdx], 
             cv::Point2f( mPoints2[ mMatches[i].trainIdx].x + mCols1, mPoints2[ mMatches[i].trainIdx].y ), 
             color, 1, CV_AA );
   }

   return mMatchesImage;

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
