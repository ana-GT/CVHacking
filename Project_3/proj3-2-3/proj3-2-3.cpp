/**
 * @file proj3-2-3.cpp
 * @brief usage: ./proj3-2-3 pts2d-pic_a.txt pts2d-pic_b.txt 
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

using namespace Eigen;

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

  /** Get the SVD decomposition. It had to be U... */
  Eigen::MatrixXd U;
  Eigen::MatrixXd V;
  Eigen::MatrixXd S(3,3);
  Eigen::MatrixXd Sr2(3,3);
  Eigen::MatrixXd Fhr2;
  
  Eigen::JacobiSVD<MatrixXd> svd( Fh, ComputeThinU | ComputeThinV);
  V = svd.matrixV();
  U = svd.matrixU();
  Eigen::VectorXd eigenvalues = svd.singularValues();

  S.row(0) << eigenvalues(0), 0, 0;
  S.row(1) << 0, eigenvalues(1), 0;
  S.row(2) << 0, 0, eigenvalues(2);

  Sr2.row(0) << eigenvalues(0), 0, 0;
  Sr2.row(1) << 0, eigenvalues(1), 0;
  Sr2.row(2) << 0, 0, 0;


  Fhr2 = U*Sr2*V.transpose();

  std::cout << "USV*: " << U*S*V.transpose() << std::endl;
  std::cout << "Fhr2: " << Fhr2 << std::endl;

   return 0; 
}


