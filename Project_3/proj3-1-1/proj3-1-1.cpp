/**
 * @file proj3-1-1.cpp
 * @brief usage: ./proj3-1-1 pts2d-pic_a.txt pts3d.txt
 * @author A. Huaman
 * @date 2011-10-17
 */

#include "normalizePoints.h"

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

   normalizePoints2D( argv[1], T2, points2D, normPoints2D );
   std::cout<< "T2: \n" << T2 << std::endl;
   

   normalizePoints3D( argv[2], T3, points3D, normPoints3D );  
   std::cout<< "T3: \n" << T3 << std::endl;
 
   return 0; 
}

