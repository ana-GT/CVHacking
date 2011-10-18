#ifndef _CALCULATE_FH_
#define _CALCULATE_FH_

#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/LU>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <climits>
#include <cfloat>


/**< SVD */
void calculateF_SVD( char* _filePoints2Da, char* _filePoints2Db, Eigen::MatrixXd &_Fh );
void calculateF_SVD( std::vector<Eigen::VectorXd> _normPoints2Da, std::vector<Eigen::VectorXd> _normPoints2Db, Eigen::MatrixXd &_Fh );


#endif /** _CALCULATE_FH_ */
