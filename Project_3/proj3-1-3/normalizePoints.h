#ifndef _NORMALIZE_POINTS_
#define _NORMALIZE_POINTS_

#include <Eigen/Core>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <climits>
#include <cfloat>


/**< Transformation 2D */
void normalizePoints2D( char* _filePoints2D, Eigen::MatrixXd &_T2, std::vector<Eigen::VectorXi> &_points2D, std::vector<Eigen::VectorXd> &_normPoints2D  );
void normalizePoints2D( std::vector<Eigen::VectorXi> _points2D, Eigen::MatrixXd &_T2, std::vector<Eigen::VectorXd> &_normPoints2D  );
/**< Transformation 3D */
void normalizePoints3D( char* _filePoints3D, Eigen::MatrixXd &_T3, std::vector<Eigen::VectorXd> &_points3D, std::vector<Eigen::VectorXd> &_normPoints3D  );
void normalizePoints3D(  std::vector<Eigen::VectorXd> _points3D,  Eigen::MatrixXd &_T3, std::vector<Eigen::VectorXd> &_normPoints3D  );


#endif /** _NORMALIZE_POINTS_ */
