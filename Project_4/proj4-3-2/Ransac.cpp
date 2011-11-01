/**
 * @file Ransac.h
 */

#include "Ransac.h"

/**
 * @function Constructor
 */
Ransac::Ransac() {

    srand ( time(NULL) );
    mSimilarity_s = 2;
    mSimilarity_P = 0.99;

    mSimilarity_e = 0.7;

    mSimilarity_N = (int) ( log( 1 - mSimilarity_P ) / log(1- pow( (1 - mSimilarity_e), mSimilarity_s )) );
    printf("N: %d \n", mSimilarity_N );
  
    mThresh2 = 15;

    rng(12345);

    mA = cv::Mat( 4, 4, CV_32FC1);
    mB = cv::Mat( 4, 1, CV_32FC1);
    mParam = cv::Mat( 4, 1, CV_32FC1);
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
 * @function RansacSimilarity
 */
void Ransac::RansacSimilarity( std::vector<cv::KeyPoint> _points1, 
															 std::vector<cv::KeyPoint> _points2,    
                               std::vector< cv::DMatch > _matches ) {

    // 1. Get the points orderly

    mSimilaritySet1.resize(0);
    mSimilaritySet2.resize(0);
 
    for( int i = 0; i < _matches.size(); i++ ) {

        cv::Point2f p1 = _points1[_matches[i].queryIdx].pt;
        cv::Point2f p2 = _points2[_matches[i].trainIdx].pt;

        mSimilaritySet1.push_back( p1 );
        mSimilaritySet2.push_back( p2 );
    }

    mSimilarity_TotalSamplesNum = mSimilaritySet1.size();

    // 2. Start algorithm

    mBestCount = 0;

    for( int i = 0; i < mSimilarity_N; i++ ) {

        // Pick a random sample
        int sampleIndex1; int sampleIndex2;
        sampleIndex1 = getRandom( mSimilarity_TotalSamplesNum ); 
        do{
            sampleIndex2 = getRandom( mSimilarity_TotalSamplesNum );
        } while( sampleIndex1 == sampleIndex2 );

        // Compute parameters
        // Yeah, this might seem moronic, but I wanna be sure it works :)

        float u1 = mSimilaritySet1[sampleIndex1].x;
        float v1 = mSimilaritySet1[sampleIndex1].y;  

        float up1 = mSimilaritySet2[sampleIndex1].x;
        float vp1 = mSimilaritySet2[sampleIndex1].y;  

        float u2 = mSimilaritySet1[sampleIndex2].x;
        float v2 = mSimilaritySet1[sampleIndex2].y;  

        float up2 = mSimilaritySet2[sampleIndex2].x;
        float vp2 = mSimilaritySet2[sampleIndex2].y;  

        mA.at<float>(0,0) = u1; mA.at<float>(0,1) = -v1; mA.at<float>(0,2) = 1; mA.at<float>(0,3) = 0;
        mA.at<float>(1,0) = v1; mA.at<float>(1,1) =  u1; mA.at<float>(1,2) = 0; mA.at<float>(1,3) = 1;
        mA.at<float>(2,0) = u2; mA.at<float>(2,1) = -v2; mA.at<float>(2,2) = 1; mA.at<float>(2,3) = 0;
        mA.at<float>(3,0) = v2; mA.at<float>(3,1) =  u2; mA.at<float>(3,2) = 0; mA.at<float>(3,3) = 1;

        mB.at<float>(0,0) = up1;         
				mB.at<float>(1,0) = vp1;
				mB.at<float>(2,0) = up2;
				mB.at<float>(3,0) = vp2; 

        svd(mA);
        svd.backSubst( mB, mParam );

        mSample_a = mParam.at<float>( 0,0 );
        mSample_b = mParam.at<float>( 1,0 );
        mSample_c = mParam.at<float>( 2,0 );
        mSample_d = mParam.at<float>( 3,0 );
    
        printf("(!) Random index: %d-%d  Sample a: %f  sample b: %f  sample c: %f  sample d: %f  \n", sampleIndex1, sampleIndex2, mSample_a, mSample_b, mSample_c, mSample_d ); 
 
        // Calculate outliers
        int count = 2;

        for( int i = 0; i <  mSimilarity_TotalSamplesNum; i++ ) {
            
            if( i == sampleIndex1 || i == sampleIndex2 ) { continue; }

            mSampleX = mSample_a*mSimilaritySet1[i].x - mSample_b*mSimilaritySet1[i].y + mSample_c;
            mSampleY = mSample_b*mSimilaritySet1[i].x + mSample_a*mSimilaritySet1[i].y + mSample_d;

            mTestX = mSimilaritySet2[i].x;
            mTestY = mSimilaritySet2[i].y;

            mError = sqrt( (mTestX - mSampleX)*(mTestX - mSampleX) + (mTestY - mSampleY)*(mTestY - mSampleY) );        
            
            //printf("(!) Test index: %d Test DX: %f  Test DY: %f Error: %f \n", i, mTestDX, mTestDY, mError);  
            if( mError < mThresh2 )
            { count++; } 
  
        }        

        if( count > mBestCount )
        {
           mBestCount = count;
           mBestSampleIndex1 = sampleIndex1;
           mBestSampleIndex2 = sampleIndex2;
           mBest_a = mSample_a; mBest_b = mSample_b;
           mBest_c = mSample_c; mBest_d = mSample_d;

        } 

        mSimilarity_e  = 1.0 - (double)count/ (double)mSimilarity_TotalSamplesNum;
        printf("--> e: %f \n", mSimilarity_e );

    }

   printf("Best count: %d out of %d best a: %f b : %f c: %f d: %f \n", mBestCount, mSimilarity_TotalSamplesNum, mBest_a, mBest_b, mBest_c, mBest_d );

    /// Now find our final parameters based in our consensus set
    mConsensusIndices.resize(0);

    mConsensusIndices.push_back(mBestSampleIndex1);
    mConsensusIndices.push_back(mBestSampleIndex2);

    // Calculate inliers
    for( int i = 0; i <  mSimilarity_TotalSamplesNum; i++ ) {
            
        if( i == mBestSampleIndex1 || i == mBestSampleIndex2 ) { continue; }

            mSampleX = mSample_a*mSimilaritySet1[i].x - mSample_b*mSimilaritySet1[i].y + mSample_c;
            mSampleY = mSample_b*mSimilaritySet1[i].x + mSample_a*mSimilaritySet1[i].y + mSample_d;

            mTestX = mSimilaritySet2[i].x;
            mTestY = mSimilaritySet2[i].y;

            mError = sqrt( (mTestX - mSampleX)*(mTestX - mSampleX) + (mTestY - mSampleY)*(mTestY - mSampleY) );        
            
            //printf("(!) Test index: %d Test DX: %f  Test DY: %f Error: %f \n", i, mTestDX, mTestDY, mError);  
            if( mError < mThresh2 )
            {  mConsensusIndices.push_back(i); }   
        } 

    // Find the best values
    mA = cv::Mat( 2*mConsensusIndices.size(), 4, CV_32FC1 );
    mB = cv::Mat( 2*mConsensusIndices.size(), 1, CV_32FC1 );

    for( int i = 0; i < mConsensusIndices.size(); i++ )
    { 
        float u = mSimilaritySet1[ mConsensusIndices[i] ].x;
        float v = mSimilaritySet1[ mConsensusIndices[i] ].y;

        float up = mSimilaritySet2[ mConsensusIndices[i] ].x;
        float vp = mSimilaritySet2[ mConsensusIndices[i] ].y;
 
        mA.at<float>(2*i, 0) = u; mA.at<float>(2*i,1) = -v; mA.at<float>(2*i,2) = 1; mA.at<float>(2*i,3) = 0;
        mA.at<float>(2*i+1,0) = v; mA.at<float>(2*i+1,1) =  u; mA.at<float>(2*i+1,2) = 0; mA.at<float>(2*i+1,3) = 1;

        mB.at<float>(2*i,0) = up;         
				mB.at<float>(2*i+1,0) = vp;
    }

    svd(mA);
    svd.backSubst( mB, mParam );
    
    mBest_a = mParam.at<float>( 0,0 );
    mBest_b = mParam.at<float>( 1,0 );
    mBest_c = mParam.at<float>( 2,0 );
    mBest_d = mParam.at<float>( 3,0 );

   printf("-->>> Best a: %f b : %f c: %f d: %f \n", mBest_a, mBest_b, mBest_c, mBest_d );

}

/**
 */
cv::Mat Ransac::drawSuperposedSimilarity(  ) {

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
       circle( mConsensusImage, mSimilaritySet1[mConsensusIndices[i]], 4, color, 2, 8, 0 );
       circle( mConsensusImage, cv::Point2f( mSimilaritySet2[mConsensusIndices[i]].x + mCols1, mSimilaritySet2[mConsensusIndices[i]].y ), 4, color, 2, 8, 0 );
       line( mConsensusImage, 
             mSimilaritySet1[mConsensusIndices[i]], 
             cv::Point2f( mSimilaritySet2[mConsensusIndices[i]].x + mCols1, mSimilaritySet2[mConsensusIndices[i]].y ), 
             color, 1, CV_AA );
   }

   return mConsensusImage;
}
