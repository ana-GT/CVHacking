/**
 * @file Gradient.cpp
 */
#include "Gradient.h"


/**
 * @function Gradient
 */
Gradient::Gradient()
{
}

/**
 * @function Init
 */
void Gradient::Init( const cv::Mat &_img )
{
   mPad = 1;

   mImage = _img;
   mRows = mImage.rows;
   mCols = mImage.cols;
   
   mPadImage = PadImage( mImage, mPad );

   // Initialize 
   mHarris = cv::Mat( mRows, mCols, CV_64FC1 ); // Double, yes

   mGradientX = cv::Mat( mRows, mCols, CV_16SC1 );
   mGradientY = cv::Mat( mRows, mCols, CV_16SC1 );
   mGradientXY = cv::Mat( mRows, mCols, CV_16SC1 );
   mGradientYX = cv::Mat( mRows, mCols, CV_16SC1 );
   mGradientXX = cv::Mat( mRows, mCols, CV_16SC1 );
   mGradientYY = cv::Mat( mRows, mCols, CV_16SC1 );
 
   mImageGradientX = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageGradientY = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageGradientXY = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageGradientYX = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageGradientXX = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageGradientYY = cv::Mat( mRows, mCols, CV_8UC1 );
   mImageHarris = cv::Mat( mRows, mCols, CV_8UC1 );

   // Sobel Filter
  mMaskX = cv::Mat( 3, 3, CV_16SC1 );
  mMaskX.at<short>(0,0) = -1; mMaskX.at<short>(0,1) = 0; mMaskX.at<short>(0,2) = 1;
  mMaskX.at<short>(1,0) = -2; mMaskX.at<short>(1,1) = 0; mMaskX.at<short>(1,2) = 2;
  mMaskX.at<short>(2,0) = -1; mMaskX.at<short>(2,1) = 0; mMaskX.at<short>(2,2) = 1;

  mMaskY = cv::Mat( 3, 3, CV_16SC1 );
  mMaskY.at<short>(0,0) = -1; mMaskY.at<short>(0,1) = -2; mMaskY.at<short>(0,2) = -1;
  mMaskY.at<short>(1,0) =  0; mMaskY.at<short>(1,1) =  0; mMaskY.at<short>(1,2) =  0;
  mMaskY.at<short>(2,0) =  1; mMaskY.at<short>(2,1) =  2; mMaskY.at<short>(2,2) =  1;

  // Harris weights
  mWeightH = cv::Mat( 3, 3, CV_16SC1 );
  mWeightH.at<short>(0,0) = 1;   mWeightH.at<short>(0,1) = 2;   mWeightH.at<short>(0,2) = 1;
  mWeightH.at<short>(1,0) = 2;   mWeightH.at<short>(1,1) = 4;   mWeightH.at<short>(1,2) = 2;
  mWeightH.at<short>(2,0) = 1;   mWeightH.at<short>(2,1) = 2;   mWeightH.at<short>(2,2) = 1;   

}
 
/**
 * @function ~Gradient
 */
Gradient::~Gradient()
{

}

/**
 * 
 */
void Gradient::GetGradients() {

  GetGradientX();
  GetGradientY();
  GetGradientXY();
  GetGradientYX();
  GetGradientXX();
  GetGradientYY();
}
   
/**
 * @function GradientX
 */
cv::Mat Gradient::GetGradientX()
{
   cv::Mat wx;

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wx = GetWindow( j,i, mPadImage, 3 );
          mGradientX.at<short>(j,i) = (short) SobelX( wx );
      }
   }    
   return mGradientX;
}


/**
 * @function GetGradientY
 */ 
cv::Mat Gradient::GetGradientY()
{
   cv::Mat wy;

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wy = GetWindow( j,i, mPadImage, 3 );
          mGradientY.at<short>(j,i) = (short) SobelY( wy );
      }
   }    

  return mGradientY;
}

/**
 * @function GradientXY
 */
cv::Mat Gradient::GetGradientXY()
{
   mPadGradientY = PadImage( mGradientY, mPad );

   cv::Mat wx;

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wx = GetWindow( j,i, mPadGradientY, 3 );
          mGradientXY.at<short>(j,i) = (short) SobelX( wx );
      }
   }    
   return mGradientXY;
}


/**
 * @function GetGradientYX
 */ 
cv::Mat Gradient::GetGradientYX()
{
   mPadGradientX = PadImage( mGradientX, mPad );

   cv::Mat wy;

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wy = GetWindow( j,i, mPadGradientX, 3 );
          mGradientYX.at<short>(j,i) = (short) SobelY( wy );
      }
   }    

  return mGradientYX;
}

/**
 * @function GradientXX
 */
cv::Mat Gradient::GetGradientXX()
{
   cv::Mat wx;

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wx = GetWindow( j,i, mPadGradientX, 3 );
          mGradientXX.at<short>(j,i) = (short) SobelX( wx );
      }
   }    
   return mGradientXX;
}


/**
 * @function GetGradientYY
 */ 
cv::Mat Gradient::GetGradientYY()
{
   cv::Mat wy;

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
          wy = GetWindow( j,i, mPadGradientY, 3 );
          mGradientYY.at<short>(j,i) = (short) SobelY( wy );
      }
   }    

  return mGradientYY;
}

/**
 * @function GetHarris
 */
cv::Mat Gradient::GetHarris( double _coeff )
{
   mCoeff = _coeff;

   double M11; double M12; double M21; double M22;
   double det; double trace;
   
   // Get Pads
   cv::Mat padXX = PadImage( mGradientXX, mPad );
   cv::Mat padYY = PadImage( mGradientYY, mPad );
   cv::Mat padXY = PadImage( mGradientXY, mPad );
   cv::Mat padYX = PadImage( mGradientYX, mPad );

   for( int j = 0; j < mRows; j++ )
   {
      for( int i = 0; i < mCols; i++ )
      {
         M11 = (double) GetWeightedSum( GetWindow(j, i, padXX, 3), mWeightH ) /16.0;
         M12 = (double) GetWeightedSum( GetWindow(j, i, padXY, 3), mWeightH ) /16.0;
         M21 = (double) GetWeightedSum( GetWindow(j, i, padYX, 3), mWeightH ) /16.0;
         M22 = (double) GetWeightedSum( GetWindow(j, i, padYY, 3), mWeightH ) /16.0;

         det = M11*M22 - M12*M21;
         trace = M11 + M22;

         mHarris.at<double>(j,i)  = (double)det - (double)mCoeff*trace*trace;

         //printf("[%d, %d] det: %f trace: %f harris: %f \n", j,i, det, trace, (double)det - (double)mCoeff*trace*trace );
         //printf("[%d, %d] Your harris is %f \n", j, i, mHarris.at<double>(j,i));

      }
   } 

   return mHarris;

}


/**
 * @function GetImageGradientX
 */
cv::Mat Gradient::GetImageGradientX()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   cv::minMaxLoc( mGradientX, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

   short minValShort = (short) minVal;
   short maxValShort = (short) maxVal;

   printf("minGx: %d \n", minValShort );
   printf("maxGx: %d \n", maxValShort );


   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientX.at<short>(j,i) - minValShort )/( maxValShort - minValShort );
         mImageGradientX.at<uchar>(j,i) = d; 
      }
   }

   return mImageGradientX;
}

/**
 * @function GetImageGradientY
 */
cv::Mat Gradient::GetImageGradientY()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   minMaxLoc( mGradientY, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );


   short minValShort = (short) minVal;
   short maxValShort = (short) maxVal;

   printf("minGy: %d \n", minValShort );
   printf("maxGy: %d \n", maxValShort );

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientY.at<short>(j,i) - minValShort )/( maxValShort - minValShort );
         mImageGradientY.at<uchar>(j,i) = d; 
      }
   }

   return mImageGradientY;
}

/**
 * @function GetImageGradientStitched
 */
cv::Mat Gradient::GetImageGradientStitched()
{
   mImageGradientStitched = cv::Mat( mRows, mCols*2, CV_8UC1 );
   
   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         mImageGradientStitched.at<uchar>(j,i) = mImageGradientX.at<uchar>(j,i);
         mImageGradientStitched.at<uchar>(j,i + mCols ) = mImageGradientY.at<uchar>(j,i);
      }
   }

   // Identify it (OPTIONAL)
   putText( mImageGradientStitched, "Gradient X", cv::Point( 0, mRows - 20 ), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0), 2, 8, false); 
   putText( mImageGradientStitched, "Gradient Y", cv::Point( mCols, mRows - 20  ), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0), 2, 8, false); 

   return mImageGradientStitched;
}

/**
 * @function GetImageGradientXY
 */
cv::Mat Gradient::GetImageGradientXY()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   minMaxLoc( mGradientXY, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );


   short minValShort = (short) minVal;
   short maxValShort = (short) maxVal;

   printf("minGxy: %d \n", minValShort );
   printf("maxGxy: %d \n", maxValShort );

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientXY.at<short>(j,i) - minValShort )/( maxValShort - minValShort );
         mImageGradientXY.at<uchar>(j,i) = d; 
      }
   }

   return mImageGradientXY;
}

/**
 * @function GetImageGradientYX
 */
cv::Mat Gradient::GetImageGradientYX()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   minMaxLoc( mGradientYX, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );


   short minValShort = (short) minVal;
   short maxValShort = (short) maxVal;

   printf("minGyx: %d \n", minValShort );
   printf("maxGyx: %d \n", maxValShort );

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientYX.at<short>(j,i) - minValShort )/( maxValShort - minValShort );
         mImageGradientYX.at<uchar>(j,i) = d; 
      }
   }

   return mImageGradientYX;
}

/**
 * @function GetImageGradientXX
 */
cv::Mat Gradient::GetImageGradientXX()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   minMaxLoc( mGradientXX, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );


   short minValShort = (short) minVal;
   short maxValShort = (short) maxVal;

   printf("minGxx: %d \n", minValShort );
   printf("maxGxx: %d \n", maxValShort );

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientXX.at<short>(j,i) - minValShort )/( maxValShort - minValShort );
         mImageGradientXX.at<uchar>(j,i) = d; 
      }
   }

   return mImageGradientXX;
}

/**
 * @function GetImageGradientYY
 */
cv::Mat Gradient::GetImageGradientYY()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   minMaxLoc( mGradientYY, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );


   short minValShort = (short) minVal;
   short maxValShort = (short) maxVal;

   printf("minGyy: %d \n", minValShort );
   printf("maxGyy: %d \n", maxValShort );

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mGradientYY.at<short>(j,i) - minValShort )/( maxValShort - minValShort );
         mImageGradientYY.at<uchar>(j,i) = d; 
      }
   }

   return mImageGradientYY;
}

/**
 * @function GetImageHarris
 */
cv::Mat Gradient::GetImageHarris()
{
   cv::Point minLoc;
   cv::Point maxLoc;
   double minVal; double maxVal;

   minMaxLoc( mHarris, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );

   for( unsigned int j = 0; j < mRows; j++ )
   {  for( unsigned int i = 0; i < mCols; i++ )
      {
         uchar d = (uchar) 255.0*( mHarris.at<double>(j,i) - minVal )/( maxVal - minVal );
         mImageHarris.at<uchar>(j,i) = d; 
      }
   }

   return mImageHarris;
}


/**
 * @function GetWindow
 */ 
cv::Mat Gradient::GetWindow( int _y, int _x, const cv::Mat &_img, int _size )
{
   cv::Mat w;

   if( _img.type() == CV_8UC1 ) {

     w = cv::Mat( 3, 3, CV_8UC1 );
     for( unsigned int j = 0; j < _size; j++ )
     {  for( unsigned int i = 0; i < _size; i++ )
        {
           w.at<uchar>(j,i) = _img.at<uchar>( _y + j,_x + i );
        }
     } 
 
   } else if( _img.type() == CV_16SC1 ) {

     w = cv::Mat( 3, 3, CV_16SC1 );
     for( unsigned int j = 0; j < _size; j++ )
     {  for( unsigned int i = 0; i < _size; i++ )
        {
           w.at<short>(j,i) = _img.at<short>( _y + j,_x + i );
        }
     } 

   }

   return w;
}

/**
 * @function SobelX
 */
int Gradient::SobelX( const cv::Mat &_wx )
{
   int sum = 0;

   if( _wx.type() == CV_8UC1 ) { 

     for( unsigned int j = 0; j < 3; j++ )
     {  for( unsigned int i = 0; i < 3; i++ )
        {
           sum += (int)_wx.at<uchar>(j,i)*( (int) mMaskX.at<short>(j,i) ); 
        }
     } 
   } else if( _wx.type() == CV_16SC1 ) {

     for( unsigned int j = 0; j < 3; j++ )
     {  for( unsigned int i = 0; i < 3; i++ )
        {
           sum += (int)_wx.at<short>(j,i)*( (int) mMaskX.at<short>(j,i) ); 
        }
     } 

   }

   return ( sum ); 
}

/**
 * @function SobelY
 */
int Gradient::SobelY( const cv::Mat &_wy )
{
   int sum = 0;

   if( _wy.type() == CV_8UC1 ) { 

     for( unsigned int j = 0; j < 3; j++ )
     {  for( unsigned int i = 0; i < 3; i++ )
        {
           sum += (int)_wy.at<uchar>(j,i)*( (int) mMaskY.at<short>(j,i) ); 
        }
     } 
   } else if( _wy.type() == CV_16SC1 ) {

     for( unsigned int j = 0; j < 3; j++ )
     {  for( unsigned int i = 0; i < 3; i++ )
        {
           sum += (int)_wy.at<short>(j,i)*( (int) mMaskY.at<short>(j,i) ); 
        }
     } 

   }

   return ( sum ); 
}


/**
 * @function GetWeightedSum
 */
int Gradient::GetWeightedSum( const cv::Mat &_window, const cv::Mat &_weight )
{
   int sum = 0;

   if( _window.type() == CV_8UC1 ) { 

     for( unsigned int j = 0; j < 3; j++ )
     {  for( unsigned int i = 0; i < 3; i++ )
        {
           sum += (int)_window.at<uchar>(j,i)*( (int) _weight.at<short>(j,i) ); 
        }
     } 
   } else if( _window.type() == CV_16SC1 ) {

     for( unsigned int j = 0; j < 3; j++ )
     {  for( unsigned int i = 0; i < 3; i++ )
        {
           sum += (int)_window.at<short>(j,i)*( (int) _weight.at<short>(j,i) ); 
        }
     } 

   }

   return ( sum ); 
}

/**
 * @function PadImage
 */
cv::Mat Gradient::PadImage( const cv::Mat &_img, int _pad )
{
   cv::Mat padImg;

   padImg = cv::Mat( _img.rows + 2*_pad, _img.cols + 2*_pad, _img.type() );
   
   cv::copyMakeBorder( _img, padImg, 
                       _pad, _pad, 
                       _pad, _pad, 
                       cv::BORDER_REPLICATE, cv::Scalar::all(0) );     

   return padImg;
}


