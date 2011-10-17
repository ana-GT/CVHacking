/**
 * @file utils.cpp
 * @author A. Huaman
 */

#include "utils.h"

/**
 * @function pickRandom
 */
void pickRandom( int _k, std::vector<int> &_randomK, int _t, std::vector<int> &_test, int _total )
{
  // srand ( time(NULL) );

   std::vector<int>pool(0);
   int badGuy;

   _randomK.resize(0);
   _test.resize(0);

   for( unsigned int i = 0; i < _total; i++ ){ pool.push_back(i); }

   //-- Get the random guys
   for( unsigned int i = 0; i < _k; i++ )
   {
      int n = pool.size();
      badGuy = rand() % n;
      _randomK.push_back( pool[badGuy] );  
  
      // Take out
      pool[badGuy] = pool[n-1]; pool.pop_back();
   }

   //-- Get the test guys (no being random :) Sing to the King eternal!
   if( _t > pool.size() ){ printf("--(!) Not enough points left to test. Check moron! \n");}
   for( unsigned int i = 0; i < _t; i++ )
   {
      int n = pool.size();
      badGuy = rand() % n;
      _test.push_back( pool[badGuy] );  
  
      // Take out
      pool[badGuy] = pool[n-1]; pool.pop_back();
   }
  
}

/**
 * @function readPoints2D
 */
void readPoints2D( char* _filePoints2D, std::vector<Eigen::VectorXi> &_points2D )
{
   int u; int v;
  
   _points2D.resize(0);

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
}

/**
 * @function readPoints3D
 */
void readPoints3D( char* _filePoints3D, std::vector<Eigen::VectorXd> &_points3D )
{
   float x; float y; float z;
  
   _points3D.resize(0);

   /** Get the data */
   FILE *pFile;
   pFile = fopen( _filePoints3D, "r" );

   if( pFile == NULL )
   { printf("--(!) Error, bad file \n");}

   while( !feof(pFile) )
   {  fscanf( pFile, "%f %f %f ", &x, &y, &z ); 
      _points3D.push_back( Eigen::Vector3d( (double)x, (double)y, (double)z ) ); 
   }  
   fclose(pFile);
}


/**
 * @function applyNorm2D
 */
void applyNorm2D( std::vector<Eigen::VectorXi> _points2D, Eigen::MatrixXd _T2,  std::vector<Eigen::VectorXd> &_normPoints2D )
{
   _normPoints2D.resize(0);
   for( unsigned int i = 0; i < _points2D.size(); i++ )
   {
     Eigen::VectorXd p2dh(3);
     Eigen::VectorXd p2dn(3);
     p2dh << (double)_points2D[i](0), (double)_points2D[i](1), 1;
     p2dn = _T2*p2dh;
     _normPoints2D.push_back( Eigen::Vector2d( p2dn(0), p2dn(1)) ); 
   }
     
}

/**
 * @function applyNorm3D
 */
void applyNorm3D( std::vector<Eigen::VectorXd> _points3D, Eigen::MatrixXd _T3,  std::vector<Eigen::VectorXd> &_normPoints3D )
{
   _normPoints3D.resize(0);
   for( unsigned int i = 0; i < _points3D.size(); i++ )
   {
     Eigen::VectorXd p3dh(4);
     Eigen::VectorXd p3dn(4);
     p3dh << _points3D[i](0), _points3D[i](1), _points3D[i](2), 1;
     p3dn = _T3*p3dh;
     _normPoints3D.push_back( Eigen::Vector3d( p3dn(0), p3dn(1), p3dn(2)) );   
   }
     
}

