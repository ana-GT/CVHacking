 cmake_minimum_required(VERSION 2.8)


project( Project4 )
include_directories( .
		     /usr/local/include
		     /usr/local/include/eigen3
                     /usr/local/include/opencv )

set (CMAKE_CXX_FLAGS "-L /usr/local/lib" )

set ( SRC_FILES proj4-1-3.cpp
                Gradient )
set ( SRC_FILES_TEST test.cpp )
set ( SRC_FILES_MY myShi.cpp )

set ( OpenCV_LIBS opencv_core
                  opencv_highgui
                  opencv_calib3d 
                  opencv_contrib
                  opencv_features2d 
                  opencv_flann )

add_executable ( proj4-1-3  ${SRC_FILES} )
add_executable ( test  ${SRC_FILES_TEST} )
add_executable ( myShi  ${SRC_FILES_MY} )
target_link_libraries( proj4-1-3 ${OpenCV_LIBS} )
target_link_libraries( test ${OpenCV_LIBS} )
target_link_libraries( myShi ${OpenCV_LIBS} )
