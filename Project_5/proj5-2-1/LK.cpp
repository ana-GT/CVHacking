/**
 */

#include "LK.h"


/**
 * @function Constructor
 */
LK::LK() {

  mKernel = cv::Mat::ones( 5,5,CV_16SC1 );
  mKernel.at<short>(0,0) = 1; mKernel.at<short>(0,1) = 4; mKernel.at<short>(0,2) = 6; mKernel.at<short>(0,3) = 4; mKernel.at<short>(0,4) = 1; 
  mKernel.at<short>(1,0) = 4; mKernel.at<short>(1,1) = 16; mKernel.at<short>(1,2) = 24; mKernel.at<short>(1,3) = 16; mKernel.at<short>(1,4) = 4; 
  mKernel.at<short>(2,0) = 6; mKernel.at<short>(2,1) = 24; mKernel.at<short>(2,2) = 36; mKernel.at<short>(2,3) = 24; mKernel.at<short>(2,4) = 6; 
  mKernel.at<short>(3,0) = 4; mKernel.at<short>(3,1) = 16; mKernel.at<short>(3,2) = 24; mKernel.at<short>(3,3) = 16; mKernel.at<short>(3,4) = 4; 
  mKernel.at<short>(4,0) = 1; mKernel.at<short>(4,1) = 4; mKernel.at<short>(4,2) = 6; mKernel.at<short>(4,3) = 4; mKernel.at<short>(4,4) = 1; 

}

/**
 * @function Destructor
 */
LK::~LK() {
}


/**
 * @function OpticFlowEstimation
 */
void LK::OpticFlowEstimation1( const cv::Mat &_img1, 
                               const cv::Mat &_img2, 
                               cv::Mat &_u,
                               cv::Mat &_v ) {

    mImg1 = _img1.clone();
    mImg2 = _img2.clone();
    mRows = mImg1.rows;
    mCols = mImg1.cols;
    mSize = 3;
    
    /// Find Gradients
    cv::Sobel( mImg1, mIx, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE );
    cv::Sobel( mImg1, mIy, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE ); 

    /// Products
    cv::multiply( mIx, mIx, mIxx );
    cv::multiply( mIx, mIy, mIxy );
    cv::multiply( mIy, mIy, mIyy );

    /// Time derivative
    mImg2_16 = cv::Mat( mImg2.rows, mImg2.cols, CV_16SC1 ) ;
    mImg1_16 = cv::Mat( mImg1.rows, mImg1.cols, CV_16SC1 ) ;

    for( int j = 0; j < mImg1.rows; j++ ) {
      for( int i = 0; i < mImg1.cols; i++ ) {
        mImg1_16.at<short>(j,i) = (short) mImg1.at<uchar>(j,i);        
        mImg2_16.at<short>(j,i) = (short) mImg2.at<uchar>(j,i);        
      }
    }

    cv::subtract( mImg2_16, mImg1_16, mIt );

    /// Products
    cv::multiply( mIx, mIt, mIxt );
    cv::multiply( mIy, mIt, mIyt );

    /// Get the sums
    cv::filter2D( mIxx, mIsxx, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIyy, mIsyy, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIxy, mIsxy, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIxt, mIsxt, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIyt, mIsyt, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

    // Do your work 
    // | a  b | u  = -m
    // | b  c | v  = -n
    mU = cv::Mat::zeros( mIx.rows, mIx.cols, CV_32FC1 );
    mV = cv::Mat::zeros( mIy.rows, mIy.cols, CV_32FC1 );

    for( int j = 0; j < mIx.rows; j++ ) {
      for( int i = 0; i < mIx.cols; i++ ) {
     
        float a = (float) mIsxx.at<short>(j,i);
        float b = (float) mIsxy.at<short>(j,i);
        float c = (float) mIsyy.at<short>(j,i);
        float m = (float) -mIsxt.at<short>(j,i);
        float n = (float) -mIsyt.at<short>(j,i);
      
        cv::SVD svd;
        cv::Mat A = cv::Mat( 2,2, CV_32FC1); 
        cv::Mat B = cv::Mat( 2,1, CV_32FC1);
        cv::Mat X = cv::Mat( 2,1, CV_32FC1);

        A.at<float>(0,0) = a; A.at<float>(0,1) = b;
        A.at<float>(1,0) = b; A.at<float>(1,1) = c;

        B.at<float>(0,0) = m;
        B.at<float>(1,0) = n;

         
        cv::Mat eigenvalues;
        cv::eigen( A, eigenvalues );
        float mThresh = 0.001;
        if( eigenvalues.at<float>(0,0) != 0 && eigenvalues.at<float>(1,0) != 0 ) { 
          if( eigenvalues.at<float>(1,0) / eigenvalues.at<float>(0,0) < mThresh ) { 
            mU.at<float>(j,i) = 0;
            mV.at<float>(j,i) = 0;
          }
        }
         
        else {
          svd(A);
          svd.backSubst( B, X );

          mU.at<float>(j,i) = X.at<float>( 0,0 )*8.0;
          mV.at<float>(j,i) = X.at<float>( 1,0 )*8.0;
        }
      }
    }

   double minVal; double maxVal;
   cv::Point minPoint; cv::Point maxPoint;
   cv::minMaxLoc( mU, &minVal, &maxVal, &minPoint, &maxPoint, cv::Mat() );
   printf("U: Min: %f -- Max: %f  Loc: Min: %d %d Max: %d %d \n", minVal, maxVal, minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

  cv::minMaxLoc( mV, &minVal, &maxVal, &minPoint, &maxPoint, cv::Mat() );
   printf("V: Min: %f -- Max: %f  Loc: Min: %d %d Max: %d %d \n", minVal, maxVal, minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

    _u = mU.clone();
    _v = mV.clone();
}


/**
 * @function OpticFlowEstimation2
 * @brief With Cramer's rule
 */
void LK::OpticFlowEstimation2( const cv::Mat &_img1, 
                              const cv::Mat &_img2, 
                              cv::Mat &_u,
                              cv::Mat &_v ) {

    mImg1 = _img1.clone();
    mImg2 = _img2.clone();
    mRows = mImg1.rows;
    mCols = mImg1.cols;
    mSize = 3;
    

    /// Find Gradients
    cv::Sobel( mImg1, mIx, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE );
    cv::Sobel( mImg1, mIy, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE ); 

    /// Products
    cv::multiply( mIx, mIx, mIxx );
    cv::multiply( mIx, mIy, mIxy );
    cv::multiply( mIy, mIy, mIyy );

    /// Time derivative
    mImg2_16 = cv::Mat( mImg2.rows, mImg2.cols, CV_16SC1 ) ;
    mImg1_16 = cv::Mat( mImg1.rows, mImg1.cols, CV_16SC1 ) ;

    for( int j = 0; j < mImg1.rows; j++ ) {
      for( int i = 0; i < mImg1.cols; i++ ) {
        mImg1_16.at<short>(j,i) = (short) mImg1.at<uchar>(j,i);        
        mImg2_16.at<short>(j,i) = (short) mImg2.at<uchar>(j,i);        
      }
    }

    cv::subtract( mImg2_16, mImg1_16, mIt );

    /// Products
    cv::multiply( mIx, mIt, mIxt );
    cv::multiply( mIy, mIt, mIyt );

 

    /// Get the sums
    cv::filter2D( mIxx, mIsxx, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIyy, mIsyy, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIxy, mIsxy, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIxt, mIsxt, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    cv::filter2D( mIyt, mIsyt, CV_16S, mKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

    // Do your work 
    // | a  b | u  = -m
    // | b  c | v  = -n
    mU = cv::Mat::zeros( mIx.rows, mIx.cols, CV_32FC1 );
    mV = cv::Mat::zeros( mIy.rows, mIy.cols, CV_32FC1 );

    for( int j = 0; j < mIx.rows; j++ ) {
      for( int i = 0; i < mIx.cols; i++ ) {
     
        float a = (float) mIsxx.at<short>(j,i);
        float b = (float) mIsxy.at<short>(j,i);
        float c = (float) mIsyy.at<short>(j,i);
        float m = (float) -mIsxt.at<short>(j,i);
        float n = (float) -mIsyt.at<short>(j,i);
      
        float delta = ( b*b - a*c);

        if( delta )
        {
           /* system is not singular - solving by Kramer method */
           float deltaX;
           float deltaY;
           float Idelta = 8 / delta;

           deltaX = (n*b - m*c);
           deltaY = (b*m - a*n);

           mU.at<float>(j,i) = deltaX * Idelta;
           mV.at<float>(j,i) = deltaY * Idelta;
        }
        else
        {
          /* singular system - find optical flow in gradient direction */
          float Norm = (b + a) * (b + a) + (c + b) * (c + b);

          if( Norm )
          {
             float IGradNorm = 8 / Norm;
             float temp = (m+n) * IGradNorm;

             mU.at<float>(j,i) = (b + a) * temp;
             mV.at<float>(j,i) = (c + b) * temp;

          }
          else
          {
            mU.at<float>(j,i) = 0;
            mV.at<float>(j,i) = 0;
          }
        }

      }
    }

   double minVal; double maxVal;
   cv::Point minPoint; cv::Point maxPoint;
   cv::minMaxLoc( mU, &minVal, &maxVal, &minPoint, &maxPoint, cv::Mat() );
   printf("U: Min: %f -- Max: %f  Loc: Min: %d %d Max: %d %d \n", minVal, maxVal, minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

  cv::minMaxLoc( mV, &minVal, &maxVal, &minPoint, &maxPoint, cv::Mat() );
   printf("V: Min: %f -- Max: %f  Loc: Min: %d %d Max: %d %d \n", minVal, maxVal, minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

    _u = mU.clone();
    _v = mV.clone();
}

/**
 * @function OpticFlowEstimation
 */
void LK::OpticFlowEstimation3( const cv::Mat &_img1, 
                              const cv::Mat &_img2, 
                              cv::Mat &_u,
                              cv::Mat &_v ) {

    mImg1 = _img1.clone();
    mImg2 = _img2.clone();
    mImg1 = _img1;
    mImg2 = _img2;
    mRows = mImg1.rows;
    mCols = mImg1.cols;
    mSize = 3;
    
    cv::Mat velx( mImg1.size(), CV_32F );
    cv::Mat vely( mImg1.size(), CV_32F ); 
    CvMat cvvelx = velx;    
    CvMat cvvely = vely;
    CvMat cvImg1 = mImg1;  // prev   
    CvMat cvImg2 = mImg2;  // curr
    cvCalcOpticalFlowLK( &cvImg1, &cvImg2, cv::Size(5,5), &cvvelx, &cvvely );

    cv::Mat flow( velx.size(), CV_32FC2 );
    for( int y = 0 ; y < flow.rows; ++y ) {
      for( int x = 0 ; x < flow.cols; ++x ) {                        
        flow.at<cv::Point2f>(y, x) = cv::Point2f(velx.at<float>(y, x), vely.at<float>(y, x)); 
      }
    }

    mU = cv::Mat( flow.size(), CV_32FC1 );
    mV = cv::Mat( flow.size(), CV_32FC1 );


    for(int y = 0 ; y < flow.rows; ++y) {
        for(int x = 0 ; x < flow.cols; ++x) {
          mU.at<float>(y,x) = flow.at<cv::Point2f>(y,x).x;
          mV.at<float>(y,x) = flow.at<cv::Point2f>(y,x).y;
       }
    } 

   double minVal; double maxVal;
   cv::Point minPoint; cv::Point maxPoint;
   cv::minMaxLoc( mU, &minVal, &maxVal, &minPoint, &maxPoint, cv::Mat() );
   printf("U: Min: %f -- Max: %f  Loc: Min: %d %d Max: %d %d \n", minVal, maxVal, minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

   cv::minMaxLoc( mV, &minVal, &maxVal, &minPoint, &maxPoint, cv::Mat() );
   printf("V: Min: %f -- Max: %f  Loc: Min: %d %d Max: %d %d \n", minVal, maxVal, minPoint.x, minPoint.y, maxPoint.x, maxPoint.y);

    _u = mU.clone();
    _v = mV.clone();
}


/**
 * @function DrawMotionArrows
 */
void LK::DrawMotionArrows( cv::Mat &_mu, 
                           cv::Mat &_mv ) {

    _mu = cv::Mat( mRows, mCols, CV_8UC3 );
    _mv = cv::Mat( mRows, mCols, CV_8UC3 );

    for( int j = 0; j < mRows; j++ ) {
      for( int i = 0; i < mCols; i++ ) {
        for( int k = 0; k < 3; k++ ) {
            _mu.at<cv::Vec3b>(j,i)[k] = 255;
            _mv.at<cv::Vec3b>(j,i)[k] = 255;
        } 
      } 
    }

    for( int j = 0; j < mRows; j++ ) {
      for( int i = 0; i < mCols; i++ ) {
  
        if( mU.at<float>(j,i) < 0 ) // Blue
        {  
          _mu.at<cv::Vec3b>(j,i)[0] = 255;
          _mu.at<cv::Vec3b>(j,i)[1] = 0;
          _mu.at<cv::Vec3b>(j,i)[2] = 0;
           //DrawHorArrow( _mu, cv::Point(i,j), -1, 4, cv::Scalar(0,0,255) ); 
        }     // BGR
        else if( mU.at<float>(j,i) > 0 ) // Magenta
        { 
          _mu.at<cv::Vec3b>(j,i)[0] = 255;
          _mu.at<cv::Vec3b>(j,i)[1] = 0;
          _mu.at<cv::Vec3b>(j,i)[2] = 255;
          //DrawHorArrow( _mu, cv::Point(i,j), 1, 4, cv::Scalar(0,255,255) ); 
        }     

        if( mV.at<float>(j,i) < 0 ) // Blue
        {  
          _mv.at<cv::Vec3b>(j,i)[0] = 255;
          _mv.at<cv::Vec3b>(j,i)[1] = 0;
          _mv.at<cv::Vec3b>(j,i)[2] = 0;
          //DrawVerArrow( _mv, cv::Point(i,j), -1, 4, cv::Scalar(0,0,255) ); 
        }      
        else if( mV.at<float>(j,i) > 0 ) // Magenta
        {  
          _mv.at<cv::Vec3b>(j,i)[0] = 255;
          _mv.at<cv::Vec3b>(j,i)[1] = 0;
          _mv.at<cv::Vec3b>(j,i)[2] = 255;
          //DrawVerArrow( _mv, cv::Point(i,j), 1, 4, cv::Scalar(0,255,255) ); 
        }  

      }
    }    


}

/**
 * @function DrawHorArrow
 */
void LK::DrawHorArrow( cv::Mat &_img, 
                       cv::Point _p, 
                       int _orient,  
                       double _length,
                       cv::Scalar _color ) {

     cv::Point endPoint;
     cv::Point endArrow1;
     cv::Point endArrow2;

     if( _orient > 0 )
     {  endPoint = cv::Point( _p.x + _length, _p.y );
        endArrow1 = cv::Point( endPoint.x - _length/4.0 , endPoint.y + _length/4.0 );
        endArrow2 = cv::Point( endPoint.x - _length/4.0 , endPoint.y - _length/4.0 );
      }
     if( _orient < 0 )
     {  endPoint = cv::Point( _p.x - _length, _p.y ); 
        endArrow1 = cv::Point( endPoint.x + _length/4.0 , endPoint.y + _length/4.0 );
        endArrow2 = cv::Point( endPoint.x + _length/4.0 , endPoint.y - _length/4.0 );
     }

     cv::line( _img, _p, endPoint, _color, 1, CV_AA );
     cv::line( _img, endPoint, endArrow1, _color, 1, CV_AA );
     cv::line( _img, endPoint, endArrow2, _color, 1, CV_AA );
}
 

/**
 * @function DrawVerArrow
 */
void LK::DrawVerArrow( cv::Mat &_img, 
                       cv::Point _p, 
                       int _orient,  
                       double _length,
                       cv::Scalar _color ) {

     cv::Point endPoint;
     cv::Point endArrow1;
     cv::Point endArrow2;

     if( _orient > 0 )
     {  endPoint = cv::Point( _p.x, _p.y + _length );
        endArrow1 = cv::Point( endPoint.x + _length/4.0 , endPoint.y - _length/4.0 );
        endArrow2 = cv::Point( endPoint.x - _length/4.0 , endPoint.y - _length/4.0 );
      }
     if( _orient < 0 )
     {  endPoint = cv::Point( _p.x, _p.y  - _length ); 
        endArrow1 = cv::Point( endPoint.x + _length/4.0 , endPoint.y + _length/4.0 );
        endArrow2 = cv::Point( endPoint.x - _length/4.0 , endPoint.y + _length/4.0 );
     }

     cv::line( _img, _p, endPoint, _color, 1, CV_AA );
     cv::line( _img, endPoint, endArrow1, _color, 1, CV_AA );
     cv::line( _img, endPoint, endArrow2, _color, 1, CV_AA );

}

/**
 * @function Remap2to1
 */
cv::Mat LK::Remap2to1( cv::Mat _img2, cv::Mat _vel_x, cv::Mat _vel_y ) {

  cv::Mat map_x, map_y;
  cv::Mat remapped;

  remapped.create( _img2.size(), CV_32FC1 );
  map_x.create( _img2.size(), CV_32FC1 );
  map_y.create( _img2.size(), CV_32FC1 );

   for( int j = 0; j < _img2.rows; j++ )
   { for( int i = 0; i < _img2.cols; i++ )
       {
             map_x.at<float>(j,i) = i + _vel_x.at<float>(j,i);
             map_y.at<float>(j,i) = j + _vel_y.at<float>(j,i);
      }
   }

  cv::remap( _img2, remapped, map_x, map_y, cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0,0, 0) );

  return remapped;
}

/**
 * @function GetDiff
 */
cv::Mat LK::GetDiff( const cv::Mat &_img2, const cv::Mat &_img1 ) {

    cv::Mat img1, img2;

   if( _img2.type() != CV_8UC1 )
   { cvtColor( _img2, img2, CV_BGR2GRAY ); }
   else
   { img2 = _img2.clone(); }

   if( _img1.type() != CV_8UC1 )
   { cvtColor( _img1, img1, CV_BGR2GRAY ); }
   else
   { img1 = _img1.clone(); }

   cv::Mat diff = cv::Mat( mRows, mCols, CV_8UC1 );

   for( int j = 0; j < mRows; j++ ) {
     for( int i = 0; i < mCols; i++ ) {
       int d = (int)( img2.at<uchar>(j,i) - img1.at<uchar>(j,i) );  
       if( d < 0 )
       {  diff.at<uchar>(j,i) = -d; }
       else
       { diff.at<uchar>(j,i) = d; }
     }
   }

  return diff;
}

