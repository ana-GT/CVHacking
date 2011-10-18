/**
 * @file calculateFh.cpp
 * @author A. Huaman
 * @date  2011-10-17
 */

#include "calculateFh.h"

using namespace Eigen;
using namespace std;


/**
 * @function  calculateF_SVD
 * @brief SVD 
 */ 
void calculateF_SVD( char* _fileNormPoints2Da, char* _fileNormPoints2Db, Eigen::MatrixXd &_Fh )
{
   vector<Eigen::VectorXd> normPoints2Da;
   vector<Eigen::VectorXd> normPoints2Db;
   float x; float y;
   int N;
   FILE *pFile;

   /**< Read 2D Points  a */
   pFile = fopen( _fileNormPoints2Da, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file, you moron! \n"); }

   while( !feof(pFile) )
   {  
      fscanf( pFile, " %f %f ", &x, &y ); // The space at the end, important!!!! 
      normPoints2Da.push_back( Eigen::Vector2d( (double)x, (double)y ) ); 
   }  
   fclose(pFile);

   /**< Read 2D Points b */
   pFile = fopen( _fileNormPoints2Db, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file, you moron! \n"); }

   while( !feof(pFile) )
   {  fscanf( pFile, "%f %f ", &x, &y ); // The space at the end, important!!!! 
      normPoints2Db.push_back( Eigen::Vector2d( (double)x, (double)y ) ); 
   }  
   fclose(pFile);

   /** Check points are the same number */
   if( normPoints2Da.size() != normPoints2Db.size() )
   { printf("--(!) Different number of points 2Da and 2Db. Check!!) \n" ); }

   N = normPoints2Db.size();
   printf("-- Number of points read: %d \n", N );

   /** Build big matrix A  */
   Eigen::MatrixXd A( N, 9 );

   /** Fill it in */
   for( unsigned int i = 0; i < N; i++ )
   {
     double xa = normPoints2Da[i][0];
     double ya = normPoints2Da[i][1];
     double xb = normPoints2Db[i][0];
     double yb = normPoints2Db[i][1];

     A.row(i) << xa*xb, xa*yb, xa, xb*ya, ya*yb, ya, xb, yb, 1;
   }

   /** It had to be U... */
   Eigen::MatrixXd V;
   JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
   V = svd.matrixV();


   Eigen::VectorXd my(9); my << 0, 0, 0, 0, 0, 0, 0, 0, 1;

   Eigen::MatrixXd m( 3, 3);
   m= V*my;

   _Fh = Eigen::MatrixXd( 3, 3 );

   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 3; i++ )  
      {
         _Fh(j,i) = m(3*j+i);
      }
   }

}

/**
 * @function calculateF_SVD 
 * @brief SVD Overloaded - Read vectors
 */ 
void calculateF_SVD( std::vector<Eigen::VectorXd> _normPoints2Da, std::vector<Eigen::VectorXd> _normPoints2Db, Eigen::MatrixXd &_Fh )
{
   int N;

   /** Check points are the same number */
   if( _normPoints2Da.size() != _normPoints2Db.size() )
   { printf("--(!) Different number of points 2Da and 2Db. Check!!) \n" ); }

   N = _normPoints2Db.size();
   printf("-- Number of points read: %d \n", N );

   /** Build big matrix A  */
   Eigen::MatrixXd A( N, 9 );

   /** Fill it in */
   for( unsigned int i = 0; i < N; i++ )
   {
     double xa = _normPoints2Da[i][0];
     double ya = _normPoints2Da[i][1];
     double xb = _normPoints2Db[i][0];
     double yb = _normPoints2Db[i][1];

     A.row(i) << xa*xb, xa*yb, xa, xb*ya, ya*yb, ya, xb, yb, 1;
   }

   /** It had to be U... */
   Eigen::MatrixXd V;
   JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
   V = svd.matrixV();


   Eigen::VectorXd my(9); my << 0, 0, 0, 0, 0, 0, 0, 0, 1;

   Eigen::MatrixXd m( 3, 3);
   m= V*my;

   _Fh = Eigen::MatrixXd( 3, 3 );

   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 3; i++ )  
      {
         _Fh(j,i) = m(3*j+i);
      }
   }

}


