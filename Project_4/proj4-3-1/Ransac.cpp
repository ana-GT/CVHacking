/**
 * @file Ransac.h
 */

#include "Ransac.h"

/**
 * @function Constructor
 */
Ransac::Ransac() {

    srand ( time(NULL) );
    mTranslation_s = 1;
    mTranslation_P = 0.99;

    mTranslation_e = 0.99;

    mTranslation_N = (int) ( log( 1 - mTranslation_P ) / log(1- pow( (1 - mTranslation_e), mTranslation_s )) );
    printf("N: %d \n", mTranslation_N );
  
    mThresh2 = 30;

    rng(12345);
}

/**
 * @function Destructor
 */
Ransac::~Ransac() {

}

/**
 * @function getRandom
 */
int Ransac::getRandom( int _numElem ) {

    return( rand() % _numElem );
}

/**
 * @function RansacTranslatin
 */
void Ransac::RansacTranslation( std::vector<cv::KeyPoint> _points1, std::vector<cv::KeyPoint> _points2,    std::vector< cv::DMatch > _matches ) {

    // 1. Get the points orderly

    mTranslationSet1.resize(0);
    mTranslationSet2.resize(0);
 
    for( int i = 0; i < _matches.size(); i++ ) {

        cv::Point2f p1 = _points1[_matches[i].queryIdx].pt;
        cv::Point2f p2 = _points2[_matches[i].trainIdx].pt;

        mTranslationSet1.push_back( p1 );
        mTranslationSet2.push_back( p2 );
    }

    mTranslation_TotalSamplesNum = mTranslationSet1.size();

    // 2. Start algorithm
    mBestCount = 0;

    for( int i = 0; i < mTranslation_N; i++ ) {

        // Pick a random sample
        int sampleIndex = getRandom( mTranslation_TotalSamplesNum ); 
    
        mSampleDX = ( mTranslationSet2[sampleIndex].x - mTranslationSet1[sampleIndex].x );
        mSampleDY = ( mTranslationSet2[sampleIndex].y - mTranslationSet1[sampleIndex].y );

        printf("(!) Random index: %d sample DX: %f  sample DY: %f \n", sampleIndex, mSampleDX, mSampleDY);  
        // Calculate outliers
        int count = 1;

        for( int i = 0; i <  mTranslation_TotalSamplesNum; i++ ) {
            
            if( i == sampleIndex ) { continue; }

            mTestDX = ( mTranslationSet2[i].x - mTranslationSet1[i].x );
            mTestDY = ( mTranslationSet2[i].y - mTranslationSet1[i].y );

            mError = sqrt( (mTestDX - mSampleDX)*(mTestDX - mSampleDX) + (mTestDY - mSampleDY)*(mTestDY - mSampleDY) );        
            
            //printf("(!) Test index: %d Test DX: %f  Test DY: %f Error: %f \n", i, mTestDX, mTestDY, mError);  
            if( mError < mThresh2 )
            { count++; } 
  
        }        

        if( count > mBestCount )
        {
           mBestCount = count;
           mBestSampleIndex = sampleIndex;
           mBestDX = mSampleDX;
           mBestDY = mSampleDY;
        } 

        mTranslation_e  = 1.0 - (double)count/ (double)mTranslation_TotalSamplesNum;
        printf("--> e: %f \n", mTranslation_e );

    }

   printf("Best count: %d out of %d best dx: %f best dy : %f \n", mBestCount, mTranslation_TotalSamplesNum, mBestDX, mBestDY);

    /// Now find our final parameters based in our consensus set
    mConsensusSetDX.resize(0);
    mConsensusSetDY.resize(0);
    mConsensusIndices.resize(0);

    mSampleDX = ( mTranslationSet2[mBestSampleIndex].x - mTranslationSet1[mBestSampleIndex].x );
    mSampleDY = ( mTranslationSet2[mBestSampleIndex].y - mTranslationSet1[mBestSampleIndex].y );

    mConsensusSetDX.push_back(mSampleDX);
    mConsensusSetDY.push_back(mSampleDY);

    mConsensusIndices.push_back(mBestSampleIndex);
    
    // Calculate inliers
    int count = 1;

    for( int i = 0; i <  mTranslation_TotalSamplesNum; i++ ) {
            
        if( i == mBestSampleIndex ) { continue; }

            mTestDX = ( mTranslationSet2[i].x - mTranslationSet1[i].x );
            mTestDY = ( mTranslationSet2[i].y - mTranslationSet1[i].y );

            mError = sqrt( (mTestDX - mSampleDX)*(mTestDX - mSampleDX) + (mTestDY - mSampleDY)*(mTestDY - mSampleDY) );        
            
            //printf("(!) Test index: %d Test DX: %f  Test DY: %f Error: %f \n", i, mTestDX, mTestDY, mError);  
            if( mError < mThresh2 )
            {
                   mConsensusSetDX.push_back(mTestDX);
                   mConsensusSetDY.push_back(mTestDY);

                   mConsensusIndices.push_back(i);
            }   
    } 



    // Find the best values
    mBestDX = 0; mBestDY = 0;

    for( int i = 0; i < mConsensusSetDX.size(); i++ )
    {  mBestDX += mConsensusSetDX[i];
       mBestDY += mConsensusSetDY[i];
    }
    mBestDX /= mConsensusSetDX.size();
    mBestDY /= mConsensusSetDY.size();

    printf("Best DX: %f DY: %f \n", mBestDX, mBestDY );

}

/**
 */
cv::Mat Ransac::drawSuperposedTranslation(  ) {

    cv::Mat trans;

    return trans;

}

/**
 * @function drawConsensusSet
 */
cv::Mat Ransac::drawConsensusSet( cv::Mat _img1, cv::Mat _img2 ) {
 
   mRows1 = _img1.rows;
   mCols1 = _img1.cols;

   mRows2 = _img2.rows;
   mCols2 = _img2.cols;

   mConsensusImage = cv::Mat( mRows1, mCols1 + mCols2, CV_8UC3 );

   for( unsigned int j = 0; j < mRows1; j++ )
   {  for( unsigned int i = 0; i < mCols1; i++ )
      {  for( int n = 0; n < 3; n++ )
         {  mConsensusImage.at<cv::Vec3b>(j,i)[n] = _img1.at<cv::Vec3b>(j,i)[n];
            mConsensusImage.at<cv::Vec3b>(j,i + mCols1 )[n] = _img2.at<cv::Vec3b>(j,i)[n];
         }
      }
   }

   for( unsigned int i = 0; i < mConsensusIndices.size(); i++ )
   {
       cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
       circle( mConsensusImage, mTranslationSet1[mConsensusIndices[i]], 4, color, 2, 8, 0 );
       circle( mConsensusImage, cv::Point2f( mTranslationSet2[mConsensusIndices[i]].x + mCols1, mTranslationSet2[mConsensusIndices[i]].y ), 4, color, 2, 8, 0 );
       line( mConsensusImage, 
             mTranslationSet1[mConsensusIndices[i]], 
             cv::Point2f( mTranslationSet2[mConsensusIndices[i]].x + mCols1, mTranslationSet2[mConsensusIndices[i]].y ), 
             color, 1, CV_AA );
   }

   return mConsensusImage;
}
