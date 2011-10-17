/**
 * @file proj3-2-2.cpp
 * @brief usage: ./proj3-2-2 pts2d-pic_a.txt pts2d-pic_b.txt 
 * @author A. Huaman
 * @date 2011-10-17
 */
#include "normalizePoints.h"
#include "calculateFh.h"

Eigen::MatrixXd Fh;

Eigen::MatrixXd T2a; 
Eigen::MatrixXd T2b;

std::vector<Eigen::VectorXi> points2Da;
std::vector<Eigen::VectorXi> points2Db;
std::vector<Eigen::VectorXd> normPoints2Da;
std::vector<Eigen::VectorXd> normPoints2Db;

/**
 * @function main 
 */
int main( int argc, char* argv[] )
{
   if( argc < 3 )
   { printf("Error. Give me two files with 2D and 3D Points \n"); return 1; }


   /** Normalize */
   normalizePoints2D( argv[1], T2a, points2Da, normPoints2Da );
   normalizePoints2D( argv[2], T2b, points2Db, normPoints2Db );  

   /** SVD */ 
   calculateF_SVD( normPoints2Da, normPoints2Db, Fh );

  /** Print Fh */
  std::cout << Fh << std::endl;

  std::cout << Fh.inverse() << std::endl;
   return 0; 
}


