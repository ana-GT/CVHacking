#ifndef _CALCULATE_M_
#define _CALCULATE_M_

#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/LU>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <climits>
#include <cfloat>


/**< Least Squares Method */
void calculateM_LSS( char* _filePoints2D, char* _filePoints3D, Eigen::MatrixXd &_M );
void calculateM_LSS( std::vector<Eigen::VectorXd> _normPoints2D, std::vector<Eigen::VectorXd> _normPoints3D, Eigen::MatrixXd &_M );
/**< SVD */
void calculateM_SVD( char* _filePoints2D, char* _filePoints3D, Eigen::MatrixXd &_M );
void calculateM_SVD( std::vector<Eigen::VectorXd> _normPoints2D, std::vector<Eigen::VectorXd> _normPoints3D, Eigen::MatrixXd &_M );
/**< Residual */
void Residual( Eigen::MatrixXd _M, std::vector<Eigen::VectorXi> _points2D, 
                                   std::vector<Eigen::VectorXd> _points3D,
                                   std::vector<Eigen::VectorXd> _normPoints2D, 
                                   std::vector<Eigen::VectorXd> _normPoints3D,
                                   Eigen::MatrixXd T2, std::vector<Eigen::VectorXd> &_residual );

#endif /** _CALCULATE_M_ */
