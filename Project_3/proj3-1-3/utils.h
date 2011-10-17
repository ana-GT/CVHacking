/**
 * @function utils.h
 */

#ifndef _PROJ3_UTILS_H_
#define _PROJ3_UTILS_H_

#include <Eigen/Core>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

/** Useful functions */
void pickRandom( int _k, std::vector<int> &_randomK, int _t, std::vector<int> &_test, int _total );
void readPoints2D( char* _filePoints2D, std::vector<Eigen::VectorXi> &_points2D );
void readPoints3D( char* _filePoints3D, std::vector<Eigen::VectorXd> &_points3D );
void applyNorm2D( std::vector<Eigen::VectorXi> _points2D, Eigen::MatrixXd _T2,  std::vector<Eigen::VectorXd> &_normPoints2D );
void applyNorm3D( std::vector<Eigen::VectorXd> _points3D, Eigen::MatrixXd _T3,  std::vector<Eigen::VectorXd> &_normPoints3D );

#endif /**< _PROJ3_UTILS_H */
