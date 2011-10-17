/**
 * @file proj3-1-3.cpp
 * @brief usage:  ./proj3-1-3 pts2d-pic_b.txt pts3d.txt
 * @author A. Huaman
 * @date 2011-10-17
 */

#include "normalizePoints.h"
#include "calculateM.h"
#include "utils.h"


Eigen::MatrixXd T2; 
Eigen::MatrixXd T3;

std::vector<Eigen::VectorXi> points2D;
std::vector<Eigen::VectorXd> points3D;
std::vector<Eigen::VectorXd> normPoints2D;
std::vector<Eigen::VectorXd> normPoints3D;


std::vector<double> storedResidual;
std::vector<Eigen::MatrixXd> storedM;
std::vector<int> storedK;


/**< Headers */
void solveQuestion3( int k, int t, int N, std::vector<Eigen::VectorXi> _points2D, 
					  std::vector<Eigen::VectorXd> _points3D, 
                                          Eigen::MatrixXd _T2, 
                                          Eigen::MatrixXd _T3,
                                          double &_sum_res,
                                          Eigen::MatrixXd &_M );


/**
 * @function main 
 */
int main( int argc, char* argv[] )
{

   if( argc < 3 )
   { printf("Error. Give me two files with 2D and 3D Points \n"); return 1; }

   srand ( time(NULL) );

   /** Read the point files */
   readPoints2D( argv[1], points2D );
   readPoints3D( argv[2], points3D );

   /** Normalize them */
   normalizePoints2D( points2D, T2, normPoints2D );
   normalizePoints3D( points3D, T3, normPoints3D );  

   int N = points2D.size();
   double sum_res;
   Eigen::MatrixXd M;

   int num_trials = 10;
   int k; int t; 

   //-- 1. k = 8 t = 5
   k = 8; t = 5; 
   for( unsigned int i = 0; i < num_trials; i++ )
   {
      solveQuestion3( k, t, N, points2D, points3D, T2, T3, sum_res, M );
      storedResidual.push_back( sum_res );
      storedM.push_back( M );
      storedK.push_back( k );    
   }

   //-- 2. k = 12 t = 5
   k = 12; t = 5; 
   for( unsigned int i = 0; i < num_trials; i++ )
   {
      solveQuestion3( k, t, N, points2D, points3D, T2, T3, sum_res, M );
      storedResidual.push_back( sum_res );
      storedM.push_back( M );
      storedK.push_back( k );    
   }

   //-- 3. k = 16 t = 4
   k = 16; t = 4; 
   for( unsigned int i = 0; i < num_trials; i++ )
   {
      solveQuestion3( k, t, N, points2D, points3D, T2, T3, sum_res, M );
      storedResidual.push_back( sum_res );
      storedM.push_back( M );
      storedK.push_back( k );    
   }

   //-- Display and find min
   double minResidual = DBL_MAX;
   int minInd = -1;

   for( unsigned int i = 0; i < storedM.size(); i++ )
   {
      printf("Trial [%d] -- K: %d Residual: %f \n", i, storedK[i], storedResidual[i]);
      if( storedResidual[i] < minResidual )
      { minResidual = storedResidual[i]; minInd = i; }
   }
  
   std::cout<< " Min residual found with k: " << storedK[minInd] << std::endl;
   std::cout<< " Min residual value: " << storedResidual[minInd] << std::endl;
   std::cout<< " Matrix M: \n" << storedM[minInd] << std::endl;


   return 0; 
}

/**
 * @function solveQuestion3
 */
void solveQuestion3( int k, int t, int N, std::vector<Eigen::VectorXi> _points2D, 
					  std::vector<Eigen::VectorXd> _points3D, 
                                          Eigen::MatrixXd _T2, 
                                          Eigen::MatrixXd _T3,
                                          double &_sum_res,
                                          Eigen::MatrixXd &_M )
{
   std::vector<Eigen::VectorXi> pointsK2D;
   std::vector<Eigen::VectorXd> pointsK3D;
   std::vector<Eigen::VectorXi> pointsTest2D;
   std::vector<Eigen::VectorXd> pointsTest3D;

   std::vector<Eigen::VectorXd> normPointsK2D;
   std::vector<Eigen::VectorXd> normPointsK3D;
   std::vector<Eigen::VectorXd> normPointsTest2D;
   std::vector<Eigen::VectorXd> normPointsTest3D;

   Eigen::MatrixXd M_SVD;

   /** Pick random points */
   std::vector<int> randomK;
   std::vector<int> test;
  
   pickRandom( k, randomK, t, test, N ); 


   /** Separate the sets */
   for( unsigned int i = 0; i < k; i++ )
   {
      pointsK2D.push_back( _points2D[ randomK[i] ] );
      pointsK3D.push_back( _points3D[ randomK[i] ] );
   }

   for( unsigned int i = 0; i < t; i++ )
   {
      pointsTest2D.push_back( _points2D[ test[i] ] );
      pointsTest3D.push_back( _points3D[ test[i] ] );
   }

   /** Normalize the 2D points with T2 */
   applyNorm2D( pointsK2D, _T2,  normPointsK2D );
   applyNorm3D( pointsK3D, _T3,  normPointsK3D );


   /** Calculate M from k points */
   calculateM_SVD( normPointsK2D, normPointsK3D, M_SVD );

   /// Residual
   applyNorm2D( pointsTest2D, _T2,  normPointsTest2D );
   applyNorm3D( pointsTest3D, _T3,  normPointsTest3D );

   std::vector<Eigen::VectorXd> residual;
   Residual( M_SVD, pointsTest2D, pointsTest3D, normPointsTest2D, normPointsTest3D, T2, residual );

   /** Output: M and Residual  */
   _sum_res = 0;
   for( unsigned int i = 0; i < residual.size(); i++ )
   { _sum_res += residual[i](2); }
   _sum_res /= residual.size();

   _M = M_SVD;
}

