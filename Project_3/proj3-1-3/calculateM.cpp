/**
 * @file normalizePoints.cpp
 */

#include "calculateM.h"

using namespace Eigen;
using namespace std;

/**
 * @function Less Square something
 */
void calculateM_LSS( char* _fileNormPoints2D, char* _fileNormPoints3D, Eigen::MatrixXd &_M )
{
   vector<Eigen::VectorXd> normPoints2D;
   vector<Eigen::VectorXd> normPoints3D;
   float x; float y; float z;
   int N;
   FILE *pFile;

   /**< Read 2D Points */
   pFile = fopen( _fileNormPoints2D, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file, you moron! \n"); }

   while( !feof(pFile) )
   {  
      fscanf( pFile, " %f %f ", &x, &y ); // The space at the end, important!!!! 
      normPoints2D.push_back( Eigen::Vector2d( (double)x, (double)y ) ); 
   }  
   fclose(pFile);

   /**< Read 3D Points */
   pFile = fopen( _fileNormPoints3D, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file, you moron! \n"); }

   while( !feof(pFile) )
   {  fscanf( pFile, "%f %f %f ", &x, &y, &z ); // The space at the end, important!!!! 
      normPoints3D.push_back( Eigen::Vector3d( (double)x, (double)y, (double)z ) ); 
   }  
   fclose(pFile);

   /** Check points are the same number */
   if( normPoints3D.size() != normPoints2D.size() )
   { printf("--(!) Different number of points 2D and 3D. Check!!) \n" ); }

   N = normPoints3D.size();
   printf("-- Number of points read: %d \n", N );

   /** Build big matrix A  and b */
   Eigen::MatrixXd A( 2*N, 11 );
   Eigen::VectorXd b( 2*N );

   /** Fill it in */
   for( unsigned int i = 0; i < N; i++ )
   {
     double X = normPoints3D[i][0];
     double Y = normPoints3D[i][1];
     double Z = normPoints3D[i][2];
     double u = normPoints2D[i][0];
     double v = normPoints2D[i][1];

     A.row(2*i) << X, Y, Z, 1, 0, 0, 0, 0, -u*X, -u*Y, -u*Z;
     A.row(2*i+1) << 0, 0, 0, 0, X, Y, Z, 1, -v*X, -v*Y, -v*Z;
     b(2*i) = u;
     b(2*i+1) = v;
   }

   Eigen::VectorXd m;
   m = A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);


   _M = Eigen::MatrixXd( 3, 4);

   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 4; i++ )  
      {
          if( j == 2 && i == 3 )
          { _M(j,i) = 1.0;  }
          else
          {  _M(j,i) = m(4*j+i); }
      }
   }


}

/**
 * @function
 * @brief Overloaded
 */
void calculateM_LSS( std::vector<Eigen::VectorXd> _normPoints2D, std::vector<Eigen::VectorXd> _normPoints3D, Eigen::MatrixXd &_M )
{
   int N;

   /** Check points are the same number */
   if( _normPoints3D.size() != _normPoints2D.size() )
   { printf("--(!) Different number of points 2D and 3D. Check!!) \n" ); }

   N = _normPoints3D.size();
   printf("-- Number of points read: %d \n", N );

   /** Build big matrix A  and b */
   Eigen::MatrixXd A( 2*N, 11 );
   Eigen::VectorXd b( 2*N );

   /** Fill it in */
   for( unsigned int i = 0; i < N; i++ )
   {
     double X =  _normPoints3D[i][0];
     double Y =  _normPoints3D[i][1];
     double Z =  _normPoints3D[i][2];
     double u =  _normPoints2D[i][0];
     double v =  _normPoints2D[i][1];

     A.row(2*i) << X, Y, Z, 1, 0, 0, 0, 0, -u*X, -u*Y, -u*Z;
     A.row(2*i+1) << 0, 0, 0, 0, X, Y, Z, 1, -v*X, -v*Y, -v*Z;
     b(2*i) = u;
     b(2*i+1) = v;
   }

   Eigen::VectorXd m;
   m = A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);


   _M = Eigen::MatrixXd( 3, 4);

   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 4; i++ )  
      {
          if( j == 2 && i == 3 )
          { _M(j,i) = 1.0;  }
          else
          {  _M(j,i) = m(4*j+i); }
      }
   }

}

/**
 * @function  calculateM_SVD
 * @brief SVD 
 */ 
void calculateM_SVD( char* _fileNormPoints2D, char* _fileNormPoints3D, Eigen::MatrixXd &_M )
{
   vector<Eigen::VectorXd> normPoints2D;
   vector<Eigen::VectorXd> normPoints3D;
   float x; float y; float z;
   int N;
   FILE *pFile;

   /**< Read 2D Points */
   pFile = fopen( _fileNormPoints2D, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file, you moron! \n"); }

   while( !feof(pFile) )
   {  
      fscanf( pFile, " %f %f ", &x, &y ); // The space at the end, important!!!! 
      normPoints2D.push_back( Eigen::Vector2d( (double)x, (double)y ) ); 
   }  
   fclose(pFile);

   /**< Read 3D Points */
   pFile = fopen( _fileNormPoints3D, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file, you moron! \n"); }

   while( !feof(pFile) )
   {  fscanf( pFile, "%f %f %f ", &x, &y, &z ); // The space at the end, important!!!! 
      normPoints3D.push_back( Eigen::Vector3d( (double)x, (double)y, (double)z ) ); 
   }  
   fclose(pFile);

   /** Check points are the same number */
   if( normPoints3D.size() != normPoints2D.size() )
   { printf("--(!) Different number of points 2D and 3D. Check!!) \n" ); }

   N = normPoints3D.size();
   printf("-- Number of points read: %d \n", N );

   /** Build big matrix A  */
   Eigen::MatrixXd A( 2*N, 12 );

   /** Fill it in */
   for( unsigned int i = 0; i < N; i++ )
   {
     double X = normPoints3D[i][0];
     double Y = normPoints3D[i][1];
     double Z = normPoints3D[i][2];
     double u = normPoints2D[i][0];
     double v = normPoints2D[i][1];

     A.row(2*i) << X, Y, Z, 1, 0, 0, 0, 0, -u*X, -u*Y, -u*Z, -u*1;
     A.row(2*i+1) << 0, 0, 0, 0, X, Y, Z, 1, -v*X, -v*Y, -v*Z, -v*1;
   }

   /** It had to be U... */
   Eigen::MatrixXd V;
   JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
   V = svd.matrixV();


   Eigen::VectorXd my(12); my << 0,0,0,0,0,0,0,0,0,0,0,1;

   Eigen::MatrixXd m( 3, 4);
   m= V*my;

   _M = Eigen::MatrixXd( 3, 4);

   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 4; i++ )  
      {
         _M(j,i) = m(4*j+i);
      }
   }

}

/**
 * @function calculateM_SVD 
 * @brief SVD Overloaded - Read vectors
 */ 
void calculateM_SVD( std::vector<Eigen::VectorXd> _normPoints2D, std::vector<Eigen::VectorXd> _normPoints3D, Eigen::MatrixXd &_M )
{
   int N;

   /** Check points are the same number */
   if( _normPoints3D.size() != _normPoints2D.size() )
   { printf("--(!) Different number of points 2D and 3D. Check!!) \n" ); }

   N = _normPoints3D.size();
   printf("-- Number of points read: %d \n", N );

   /** Build big matrix A  */
   Eigen::MatrixXd A( 2*N, 12 );

   /** Fill it in */
   for( unsigned int i = 0; i < N; i++ )
   {
     double X = _normPoints3D[i][0];
     double Y = _normPoints3D[i][1];
     double Z = _normPoints3D[i][2];
     double u = _normPoints2D[i][0];
     double v = _normPoints2D[i][1];

     A.row(2*i) << X, Y, Z, 1, 0, 0, 0, 0, -u*X, -u*Y, -u*Z, -u*1;
     A.row(2*i+1) << 0, 0, 0, 0, X, Y, Z, 1, -v*X, -v*Y, -v*Z, -v*1;
   }

   /** It had to be U... */
   Eigen::MatrixXd V;
   JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
   V = svd.matrixV();


   Eigen::VectorXd my(12); my << 0,0,0,0,0,0,0,0,0,0,0,1;

   Eigen::MatrixXd m( 3, 4);
   m= V*my;

   _M = Eigen::MatrixXd( 3, 4);

   for( unsigned int j = 0; j < 3; j++ )
   {  for( unsigned int i = 0; i < 4; i++ )  
      {
         _M(j,i) = m(4*j+i);
      }
   }


}



/**
 * @function Residual 
 */
void Residual( Eigen::MatrixXd _M, std::vector<Eigen::VectorXi> _points2D, 
                                   std::vector<Eigen::VectorXd> _points3D,
                                   std::vector<Eigen::VectorXd> _normPoints2D, 
                                   std::vector<Eigen::VectorXd> _normPoints3D,
                                   Eigen::MatrixXd _T2,
                                   std::vector<Eigen::VectorXd> &_residual )
{
   int N = _points2D.size();
   Eigen::VectorXd uvh;
   double u; double v;
   std::vector<Eigen::VectorXd> uv_normPredicted;
   std::vector<Eigen::VectorXd> uv_predicted;
   std::vector<Eigen::VectorXd> normResidual;
   Eigen::MatrixXd T2inv;

   _residual.resize(0);

   for( int i = 0; i < N; i++ )
   {
      Eigen::VectorXd normPoint3D_h(4);
      normPoint3D_h << _normPoints3D[i](0), _normPoints3D[i](1), _normPoints3D[i](2), 1; 
      uvh = _M*normPoint3D_h;
      u = uvh(0) / uvh(2);  v = uvh(1) / uvh(2);

      uv_normPredicted.push_back( Eigen::Vector2d( u, v ) );
      double nru = u - _normPoints2D[i](0); 
      double nrv = v - _normPoints2D[i](1);
      double nrd = sqrt( nru*nru + nrv*nrv );
      normResidual.push_back( Eigen::Vector3d( nru, nrv, nrd) );

      T2inv = _T2.inverse();
      Eigen::VectorXd uvp = T2inv*Eigen::Vector3d( u, v, 1 );
      double ru = uvp(0) - _points2D[i](0); 
      double rv = uvp(1) - _points2D[i](1);
      double rd = sqrt( ru*ru + rv*rv );
      uv_predicted.push_back( Eigen::Vector2d( uvp(0), uvp(1) ) );
      _residual.push_back( Eigen::Vector3d( ru, rv, rd ) );

   }


   /** Display */
   
   printf("Residual!! \n");
   for( unsigned int i = 0; i < N; i++ )
   {
      printf("[%d] P2D: (%d , %d) -- Predicted: (%.4f , %.4f) - Residual: (%.4f %.4f) Mod: %.4f \n",i, _points2D[i](0), _points2D[i](1), uv_predicted[i](0), uv_predicted[i](1), _residual[i](0), _residual[i](1), _residual[i](2));
   }
 
}
