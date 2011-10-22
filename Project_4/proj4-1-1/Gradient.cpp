/**
 * @file Gradient.cpp
 */
#include "Gradient.h"


/**
 * @function Gradient
 */
Gradient::Gradient( cv::Mat _img )
{
   mPad = 1;

   mImage = _img;
   mRows = mImage.rows;
   mCols = mImage.cols;

   mPadImage = cv::Mat( mRows + 2*mPad, mCols + 2*mPad, CV_8UC1 );
   
   cv::copyMakeBorder( mImage, mPadImage, 
                       mPad, mPad, 
                       mPad, mPad, 
                       cv::BORDER_REPLICATE, cv::Scalar::all(0) );   

   // Initialize 
   mGradientX = new int[mRows*mCols];
   mGradientY = new int[mRows*mCols];
 
   mImageGradientX = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageGradientY = cv::Mat( mRows, mCols, CV_8UC1 );

   // Mask
  mMaskX = new int[9];
  mMaskX[0] = -1; mMaskX[1] = 0; mMaskX[2] = 1;
  mMaskX[3] = -2; mMaskX[4] = 0; mMaskX[5] = 2;
  mMaskX[6] = -1; mMaskX[7] = 0; mMaskX[8] = 1;

  mMaskY = new int[9];
  mMaskY[0] = -1; mMaskY[1] = -2; mMaskY[2] = -1;
  mMaskY[3] =  0; mMaskY[4] =  0; mMaskY[5] =  0;
  mMaskY[6] =  1; mMaskY[7] =  2; mMaskY[8] =  1;

}
 
/**
 * @function ~Gradient
 */
Gradient::~Gradient()
{
   if( mGradientX != NULL )
   { delete [] mGradientX; }

   if( mGradientY != NULL )
   { delete [] mGradientY; }
}
   
/**
 * @function GradientX
 */
int* Gradient::GetGradientX()
{
   int *wx;
   int ind;

   ind = 0;
   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wx = GetWindow3( j,i );
          mGradientX[ind] = SobelX( wx );
          ind++;       
      }
   }    
   return mGradientX;
}


/**
 * @function GetGradientY
 */ 
int* Gradient::GetGradientY()
{
   int *wy;
   int ind;

   ind = 0;
   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wy = GetWindow3( j,i );
          mGradientY[ind] = SobelY( wy );
          ind++;
      }
   }    

  return mGradientY;
}

/**
 * @function GetImageGradientX
 */
cv::Mat Gradient::GetImageGradientX()
{
   GetGradientX();
   int minGX = *std::min_element( mGradientX, mGradientX + mRows*mCols );
   int maxGX = *std::max_element( mGradientX, mGradientX + mRows*mCols );

   printf("minGx: %d \n", minGX );
   printf("maxGx: %d \n", maxGX );

   int ind;

   ind = 0;
   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientX[ind] - minGX )/( maxGX - minGX );
         mImageGradientX.at<uchar>(j,i) = d; 
         ind++; 
      }
   }

   return mImageGradientX;
}

/**
 * @function GetImageGradientY
 */
cv::Mat Gradient::GetImageGradientY()
{
   GetGradientY();
   int minGY = *std::min_element( mGradientY, mGradientY + mRows*mCols );
   int maxGY = *std::max_element( mGradientY, mGradientY + mRows*mCols );

   printf("minGY: %d \n", minGY );
   printf("maxGY: %d \n", maxGY );

   int ind;

   ind = 0;
   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientY[ind] - minGY )/( maxGY - minGY );
         mImageGradientY.at<uchar>(j,i) = d; 
         ind++; 
      }
   }

   return mImageGradientY;
}

/**
 * @function GetImageGradientXY
 */
cv::Mat Gradient::GetImageGradientXY()
{
   GetGradientX();
   GetGradientY();

   mImageGradientXY = cv::Mat( mRows, mCols*2, CV_8UC1 );
   
   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         mImageGradientXY.at<uchar>(j,i) = mImageGradientX.at<uchar>(j,i);
         mImageGradientXY.at<uchar>(j,i + mCols ) = mImageGradientY.at<uchar>(j,i);
      }
   }

   // Identify it (OPTIONAL)
   putText( mImageGradientXY, "Gradient X", cv::Point( 0, mRows - 20 ), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0), 2, 8, false); 
   putText( mImageGradientXY, "Gradient Y", cv::Point( mCols, mRows - 20  ), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0), 2, 8, false); 

   return mImageGradientXY;
}


/**
 * @function GetWindow3
 */ 
int* Gradient::GetWindow3( int _y, int _x )
{
   int *w = new int[9];
   int ind;

   ind = 0;
   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 3; i++ )
      {
         w[ind] = mPadImage.at<uchar>( _y + j,_x + i );
         ind++;
      }
   } 

   return w;
}

/**
 * @function SobelX
 */
int Gradient::SobelX( int* _wx )
{
   int sum = 0;

   for( unsigned int i = 0; i < 9; i++ )
   {   
      sum += (_wx[i])*mMaskX[i]; 
     
   }
   return ( sum ); 
}

/**
 * @function SobelY
 */
int Gradient::SobelY( int* _wy )
{
   int sum = 0;

   for( unsigned int i = 0; i < 9; i++ )
   {   
      sum += (_wy[i])*mMaskY[i]; 
   }
   return ( sum ); 
}
