/**
 * @file proj3-2-1.cpp
 * @brief usage: ./proj3-2-1 pts2d-pic_a.txt pts2d-pic_b.txt
 * @author A. Huaman
 * @date 2011-10-17
 */

#include "normalizePoints.h"

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
   { printf("Error. Give me two files with 2D Points (A and B) \n"); return 1; }

   normalizePoints2D( argv[1], T2a, points2Da, normPoints2Da );
   std::cout<< "T2a: \n" << T2a << std::endl;
   

   normalizePoints2D( argv[2], T2b, points2Db, normPoints2Db );  
   std::cout<< "T2b: \n" << T2b << std::endl;
 
   return 0; 
}

