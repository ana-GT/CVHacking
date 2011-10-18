/**
 * @file proj3-1-4.cpp
 * @brief usage: ./proj3-1-4 
 * @author A. Huaman
 * @date 2011-10-17
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <Eigen/Core>
#include <Eigen/LU>

using namespace cv;
/**
 * @function main 
 */
int main( int argc, char* argv[] )
{
   /** Hard-coded M from last part */
  Eigen::MatrixXd M(3,4);
  Eigen::MatrixXd T3(4,4);
  Eigen::MatrixXd Q(3,3);
  Eigen::VectorXd m4(3);
  Eigen::VectorXd C_hat(3);
  Eigen::VectorXd C(3);

  /** Divide it */
  M.row(0) << -0.178791,  0.503007, 0.0121929, 0.0134698;
  M.row(1) << 0.0871845,  0.020935,  0.527886, 0.0273949;
  M.row(2) << -0.271446, -0.132274, 0.0673814, -0.575773;

  Q = M.block<3,3>(0,0);
  m4 = M.block<3,1>(0,3);

  /** T3: */
  T3.row(0) << 0.283844, 0, 0, -87.7714;
  T3.row(1) << 0, 0.283844, 0, -87.8424;
  T3.row(2) << 0, 0, 0.283844, -8.37431;
  T3.row(3) << 0, 0, 0,  1;

  C_hat = Q.inverse()*m4;

  Eigen::VectorXd C_hatH(4);
  C_hatH << C_hat(0), C_hat(1), C_hat(2), 1; 
  C = T3.inverse()*C_hatH;

  std::cout<< "M: \n" << M<< std::endl;
  std::cout<< "Q: \n" << Q<< std::endl;
  std::cout<< "m4 : \n" << m4 << std::endl;

  std::cout<< "T3 : \n" << T3 << std::endl;
  std::cout<< "T3 inv : \n" << T3.inverse() << std::endl;
  std::cout<< "C hat: \n" << C_hat << std::endl;
  std::cout<< "C: \n" << C << std::endl;

}

