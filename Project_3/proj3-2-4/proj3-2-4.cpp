/**
 * @file proj3-2-4.cpp
 * @brief usage: ./proj3-2-4 pts2d-pic_a.txt pts2d-pic_b.txt 
 * @author A. Huaman
 * @date 2011-10-17
 */
#include "normalizePoints.h"
#include "calculateFh.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <stdlib.h>
#include <stdio.h>

cv::RNG rng(12345);

Eigen::MatrixXd Fha;
Eigen::MatrixXd Fhb;

Eigen::MatrixXd Fhr2a;
Eigen::MatrixXd Fhr2b;

Eigen::MatrixXd Fa;
Eigen::MatrixXd Fb;


Eigen::MatrixXd T2a; 
Eigen::MatrixXd T2b;

std::vector<Eigen::VectorXi> points2Da;
std::vector<Eigen::VectorXi> points2Db;
std::vector<Eigen::VectorXd> normPoints2Da;
std::vector<Eigen::VectorXd> normPoints2Db;

char filenamea[50] = "pic_a.jpg";
char filenameb[50] = "pic_b.jpg";
char windowa[50] = "Window a";
char windowb[50] = "Window b";



using namespace Eigen;
using namespace cv;

void  reduceRank2( Eigen::MatrixXd _Fh, Eigen::MatrixXd &_Fhr2 );
void drawEpipolar( cv::Mat _imga, cv::Mat _imgb, 
                   std::vector<Eigen::VectorXi> _p2Da, 
                   std::vector<Eigen::VectorXi> _p2Db, 
                   std::vector<Eigen::VectorXd> _pda,
                   std::vector<Eigen::VectorXd> _pdb );

/**
 * @function main 
 */
int main( int argc, char* argv[] )
{
   if( argc < 3 )
   { printf("Error. Give me two files with 2D and 3D Points \n"); return 1; }

    cv::Mat pic_a = imread( filenamea, 1);
    cv::Mat pic_b = imread( filenameb, 1);

   /** Normalize */
   normalizePoints2D( argv[1], T2a, points2Da, normPoints2Da );
   normalizePoints2D( argv[2], T2b, points2Db, normPoints2Db );  

   /** SVD a-b */ 
   calculateF_SVD( normPoints2Da, normPoints2Db, Fha );
   calculateF_SVD( normPoints2Db, normPoints2Da, Fhb );

  /** Print Fh */
  std::cout << Fha << std::endl;
  std::cout << Fhb << std::endl;

  /** Check normalized error */
  std::cout <<"Normalized errors: " << std::endl;
/*
  for( int i = 0; i < points2Da.size(); i++ )
  {
     Eigen::VectorXd normResa = Eigen::Vector3d( normPoints2Da[i](0), normPoints2Da[i](1),1).transpose()*Fha*Eigen::Vector3d(normPoints2Db[i](0), normPoints2Db[i](1), 1);
     Eigen::VectorXd normResb = Eigen::Vector3d( normPoints2Db[i](0), normPoints2Db[i](1),1).transpose()*Fhb*Eigen::Vector3d(normPoints2Da[i](0), normPoints2Da[i](1), 1);
     std::cout<<" [" << i<<"] Error a: " << normResa(0) << " Error b: "<< normResb(0)  << std::endl;
  }
*/
  /** Reduce to rank 2 -- the wonderful SVD... */
  reduceRank2( Fha, Fhr2a );
  reduceRank2( Fhb, Fhr2b );

  std::cout << " Fhr2a: \n" << Fhr2a << std::endl;
  std::cout << " Fhr2b: \n" << Fhr2b << std::endl;

  /** Now the real big F */
  Fa = T2a.transpose()*Fhr2a*T2b; /** We see points in image a */
  Fb = T2b.transpose()*Fhr2b*T2a; /** We see points in image b */

  std::cout <<"Fa: \n" << Fa << std::endl;
  std::cout <<"Fb: \n" << Fb << std::endl;


  /** Find lines */
  int N = points2Db.size();

  std::vector<Eigen::VectorXd> linesa;
  std::vector<Eigen::VectorXd> linesb;

  linesa.resize(0);
  linesb.resize(0);

  for( unsigned int i = 0; i < N; i++ )
  {
     Eigen::VectorXd la; 
     la = Fa*Eigen::Vector3d( (double)points2Db[i](0), (double)points2Db[i](1), 1 );
     Eigen::VectorXd lb; 
     lb = Fb*Eigen::Vector3d( (double)points2Da[i](0), (double)points2Da[i](1), 1 );
     linesa.push_back( la );
     linesb.push_back( lb );
     std::cout << "[" << i <<"] la: " << la(0) << "," << la(1) << "," << la(2) << std::endl;
     std::cout << "[" << i <<"] lb: " << lb(0) << "," << lb(1) << "," << lb(2) << std::endl;
  }

 /// DEBUGGING
/*
  /// Checking
  for( unsigned int i = 0; i < N; i++ )
  {
    Eigen::VectorXd errorla = (linesa[i].transpose())*Eigen::Vector3d( (double)points2Da[i](0), (double)points2Da[i](1), 1 );
    Eigen::VectorXd errorlb = (linesb[i].transpose())*Eigen::Vector3d( (double)points2Db[i](0), (double)points2Db[i](1), 1 );
    std::cout << "[" << i <<"] la error:" << errorla(0) << " lb error: "<< errorlb(0)<<std::endl;
  }


  /// See how they match 
  for( unsigned int i = 0; i < N; i++ )
  {  double x = (double) points2Db[i](0);
     double y = (-linesb[i](0)*x  -linesb[i](2))/linesb[i](1);
     std::cout << "xy b:" << x <<" , "<< y << std::endl;
   }
  
  /// See how they match 
  for( unsigned int i = 0; i < N; i++ )
  {  double x = (double) points2Da[i](0);
     double y = (-linesa[i](0)*x  -linesa[i](2))/linesa[i](1);
     std::cout << "xy a:" << x <<" , "<< y << std::endl;
   }
*/

  /** Find points to draw */
  std::vector<Eigen::VectorXd> pointsa;
  std::vector<Eigen::VectorXd> pointsb;

  for( unsigned int i = 0; i < N; i++ )
  {
     double xmax = (double) points2Da[i](0) + pic_a.cols ;
     double xmin = (double) points2Da[i](0) - pic_a.cols ;

     double ymax = (-linesa[i](0)*xmax  -linesa[i](2))/linesa[i](1);
     double ymin = (-linesa[i](0)*xmin  -linesa[i](2))/linesa[i](1);
     
     pointsa.push_back( Eigen::Vector4d( xmin, ymin, xmax, ymax ) );
   
     std::cout << "PA: ["<<i<<"]  min: " << xmin << "," << ymin << "-- max: " << xmax << " , " <<ymax << std::endl;

  }

  for( unsigned int i = 0; i < N; i++ )
  {
     double xmax = (double) points2Db[i](0) + pic_b.cols ;
     double xmin = (double) points2Db[i](0) - pic_b.cols ;

     double ymax = (-linesb[i](0)*xmax  -linesb[i](2))/linesb[i](1);
     double ymin = (-linesb[i](0)*xmin  -linesb[i](2))/linesb[i](1);
     
     pointsb.push_back( Eigen::Vector4d( xmin, ymin, xmax, ymax ) );

     std::cout << "PB: ["<<i<<"]  min: " << xmin << " , " << ymin << "-- max: " << xmax << " , " <<ymax << std::endl;

  }

  /** Show images */  
  cv::Mat drawa = pic_a;
  cv::Mat drawb = pic_b;

  /** Draw points */
  drawEpipolar( drawa, drawb, points2Da, points2Db, pointsa, pointsb  );

  /** Save images */
  imwrite( "epipolarLines_a.png", drawa );
  imwrite( "epipolarLines_b.png", drawb );

  namedWindow( windowa, CV_WINDOW_NORMAL ); 
  namedWindow( windowb, CV_WINDOW_NORMAL ); 
  imshow( windowa, drawa );
  imshow( windowb, drawb );


  waitKey(0);

   return 0; 
}

/**
 * @function drawPoints
 */

void drawEpipolar( cv::Mat _imga, cv::Mat _imgb, 
                   std::vector<Eigen::VectorXi> _p2Da, 
                   std::vector<Eigen::VectorXi> _p2Db, 
                   std::vector<Eigen::VectorXd> _pda,
                   std::vector<Eigen::VectorXd> _pdb )
{

   char num[] = "12";

   for( unsigned int i = 0; i < _p2Da.size(); i++ )
   {  
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );
      sprintf( num, "%d ", i);

      circle( _imga, Point( _p2Da[i](0), _p2Da[i](1)), 5, color, -1, 8, 0 );
      putText( _imga, num, Point( _p2Da[i](0), _p2Da[i](1) ), FONT_HERSHEY_SIMPLEX, 1, color, 2, 8, false); 

      sprintf( num, "%d ", i);
      circle( _imgb, Point( _p2Db[i](0), _p2Db[i](1)), 5, color, -1, 8, 0 );
      putText( _imgb, num, Point( _p2Db[i](0), _p2Db[i](1) ), FONT_HERSHEY_SIMPLEX, 1, color, 2, 8, false); 

      Point2f pt1a; Point2f pt2a;
      pt1a.x = _pda[i](0); pt1a.y = _pda[i](1);
      pt2a.x = _pda[i](2); pt2a.y = _pda[i](3);

      line( _imga, pt1a, pt2a, color, 2, CV_AA );

      Point2f pt1b; Point2f pt2b;
      pt1b.x = _pdb[i](0); pt1b.y = _pdb[i](1);
      pt2b.x = _pdb[i](2); pt2b.y = _pdb[i](3);

      line( _imgb, pt1b, pt2b, color, 2, CV_AA );
   }    
}

/**
 * @function reduceRank2
 */
void  reduceRank2( Eigen::MatrixXd _Fh, Eigen::MatrixXd &_Fhr2 )
{
  /** Get the SVD decomposition. It had to be U... */
  Eigen::MatrixXd U;
  Eigen::MatrixXd V;
  Eigen::MatrixXd S(3,3);
  Eigen::MatrixXd Sr2(3,3);
  Eigen::MatrixXd Fhr2;
  
  Eigen::JacobiSVD<MatrixXd> svd( _Fh, ComputeThinU | ComputeThinV);
  V = svd.matrixV();
  U = svd.matrixU();
  Eigen::VectorXd eigenvalues = svd.singularValues();

  S.row(0) << eigenvalues(0), 0, 0;
  S.row(1) << 0, eigenvalues(1), 0;
  S.row(2) << 0, 0, eigenvalues(2);

  Sr2.row(0) << eigenvalues(0), 0, 0;
  Sr2.row(1) << 0, eigenvalues(1), 0;
  Sr2.row(2) << 0, 0, 0;


  _Fhr2 = U*Sr2*V.transpose();
     
}


