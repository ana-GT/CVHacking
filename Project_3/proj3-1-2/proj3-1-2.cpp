/**
 * @file proj3-1-2.cpp
 * @brief usage: DEBUG: ./proj3-1-2 pts2d-norm-pic_a.txt pts3d-norm.txt ANSWER: ./proj3-1-2 pts2d-pic_a.txt pts3d.txt
 * @author A. Huaman
 * @date 2011-10-17
 */
#include "normalizePoints.h"
#include "calculateM.h"

Eigen::MatrixXd M_LSS;
Eigen::MatrixXd M_SVD;

Eigen::MatrixXd T2; 
Eigen::MatrixXd T3;

std::vector<Eigen::VectorXi> points2D;
std::vector<Eigen::VectorXd> points3D;
std::vector<Eigen::VectorXd> normPoints2D;
std::vector<Eigen::VectorXd> normPoints3D;

/**
 * @function main 
 */
int main( int argc, char* argv[] )
{
   if( argc < 3 )
   { printf("Error. Give me two files with 2D and 3D Points \n"); return 1; }

/*
   /// DEBUG

   /// Least Squares Method 
   calculateM_LSS( argv[1], argv[2], M_LSS );

   std::cout<< "M_LSS: \n" << M_LSS << std::endl;

   /// SVD 
   calculateM_SVD( argv[1], argv[2], M_SVD );
   std::cout<< "M_SVD: \n" << M_SVD << std::endl;
*/

   /** Normalize */
   normalizePoints2D( argv[1], T2, points2D, normPoints2D );
   normalizePoints3D( argv[2], T3, points3D, normPoints3D );  

   /** Find Matrix */
   calculateM_LSS( normPoints2D, normPoints3D, M_LSS );

   /// LSS
   std::cout<< "M_LSS: \n" << M_LSS << std::endl;

   /// SVD 
   calculateM_SVD( normPoints2D, normPoints3D, M_SVD );
   std::cout<< "M_SVD: \n" << M_SVD << std::endl;

   Residual( M_SVD, points2D, points3D, normPoints2D, normPoints3D, T2 );


   return 0; 
}


