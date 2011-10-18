/**
 * @file normalizePoints.cpp
 */

#include "normalizePoints.h"

using namespace std;

/**
 * @function normalizePoints2D 
 */
void normalizePoints2D( char* _filePoints2D, Eigen::MatrixXd &_T2, std::vector<Eigen::VectorXi> &_points2D, std::vector<Eigen::VectorXd> &_normPoints2D )
{
   _points2D.resize(0);

   int u; int v;
   int sum_u; int sum_v; int n;
   double max_u;

   double c_u; double c_v; double s;

   /** Get the data */
   FILE *pFile;
   pFile = fopen( _filePoints2D, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file \n");}

   while( !feof(pFile) )
   {  fscanf( pFile, "%d %d ", &u, &v ); 
      _points2D.push_back( Eigen::Vector2i( u, v ) ); 
   }  
   fclose(pFile);

   n = _points2D.size();

   /** Find c_u and c_v */
   sum_u = 0;  sum_v = 0;
   for( unsigned int i = 0; i < n; i++ )
   {  sum_u += _points2D[i][0];
      sum_v += _points2D[i][1];
   }
 
   c_u = (double) sum_u / n;
   c_v = (double) sum_v / n;

   /** Find scale factor s */
   max_u = -DBL_MAX;

   for( unsigned int i = 0; i < n; i++ )
   {  if( (_points2D[i][0] - c_u) > max_u ) 
      {  max_u = _points2D[i][0]- c_u; }
   }
   
   s = 1.0/max_u;

   /** Find matrix */
   Eigen::Matrix3d Ts; Ts << s, 0.0, 0.0, 0.0, s, 0.0, 0.0, 0.0, 1.0;
   Eigen::Matrix3d Tf; Tf << 1.0, 0.0, -c_u, 0.0,1.0,-c_v, 0.0, 0.0, 1.0; 
   std::cout <<"* number of points: " << n << std::endl; 
   std::cout <<"* s: " << s << std::endl;
   std::cout <<"* c_u : " << c_u << std::endl;
   std::cout <<" * c_v: " << c_v << std::endl;

   /** Output */
   _T2 = Ts*Tf;

  _normPoints2D.resize(0);
  Eigen::VectorXd p2dn;
  Eigen::VectorXd p2dh(3); 

  for( unsigned int i = 0; i < n; i++ )
  {
    p2dh << _points2D[i](0), _points2D[i](1), 1;
    p2dn = _T2*p2dh;
    _normPoints2D.push_back( Eigen::Vector2d( p2dn(0), p2dn(1) ) );  
  }


}

/**
 * @function normalizePoints3D 
 */
void normalizePoints3D( char* _filePoints3D, Eigen::MatrixXd &_T3, std::vector<Eigen::VectorXd> &_points3D, std::vector<Eigen::VectorXd> &_normPoints3D  )
{

   _points3D.resize(0);

   float u; float v; float w;
   double sum_u; double sum_v; double sum_w; int n;
   double max_u;

   double c_u; double c_v; double c_w; double s;

   /** Get the data */
   FILE *pFile3D;
   pFile3D = fopen( _filePoints3D, "r" );

   if( pFile3D == NULL )
   { printf("--(!) Error, bad file \n");}

   while( !feof(pFile3D) )
   {  fscanf( pFile3D, "%f %f %f ", &u, &v, &w ); // The space at the end, important!!!! 
      _points3D.push_back( Eigen::Vector3d( (double)u, (double)v, (double)w ) ); 
   }  
   fclose(pFile3D);

   n = _points3D.size();

   /** Find c_u and c_v */
   sum_u = 0;  sum_v = 0; sum_w = 0;
   for( unsigned int i = 0; i < n; i++ )
   {  sum_u += _points3D[i][0];
      sum_v += _points3D[i][1];
      sum_w += _points3D[i][2];
   }
 
   c_u = (double) sum_u / n;
   c_v = (double) sum_v / n;
   c_w = (double) sum_w / n;

   /** Find scale factor s */
   max_u = -DBL_MAX;

   for( unsigned int i = 0; i < n; i++ )
   {  if( (_points3D[i][0] - c_u) > max_u ) 
      {  max_u = _points3D[i][0]- c_u; }
   }
   
   s = 1.0/max_u;

   /** Find matrix */
   Eigen::Matrix4d Ts; Ts << s, 0.0, 0.0, 0.0,  0.0, s, 0.0, 0.0,  0.0, 0.0, s, 0.0, 0.0, 0.0, 0.0, 1.0;
   Eigen::Matrix4d Tf; Tf << 1.0, 0.0, 0.0, -c_u, 0.0, 1.0, 0.0, -c_v, 0.0, 0.0, 1.0, -c_w, 0.0, 0.0, 0.0, 1.0; 
   std::cout <<"* number of points: " << n << std::endl;   
   std::cout <<"* s: " << s << std::endl;
   std::cout <<"* c_u : " << c_u << std::endl;
   std::cout <<" * c_v: " << c_v << std::endl;
   std::cout <<" * c_w: " << c_w << std::endl;

   /** Output */
   _T3 = Ts*Tf;

  _normPoints3D.resize(0);
  Eigen::VectorXd p3dn;
  Eigen::VectorXd p3dh(4); 

  for( unsigned int i = 0; i < n; i++ )
  {
    p3dh << _points3D[i](0), _points3D[i](1), _points3D[i](2), 1;
    p3dn = _T3*p3dh;
    _normPoints3D.push_back( Eigen::Vector3d( p3dn(0), p3dn(1), p3dn(2) ) );  
  }

}

