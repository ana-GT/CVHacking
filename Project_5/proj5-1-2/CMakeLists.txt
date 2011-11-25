 cmake_minimum_required(VERSION 2.8)


project( Project5 )
include_directories( .
		     /usr/local/include
		     /usr/local/include/eigen3
                     /usr/local/include/opencv )

set (CMAKE_CXX_FLAGS "-L /usr/local/lib" )

set ( SRC_FILES proj5-1-2
                Pyramid
               )
set ( OpenCV_LIBS opencv_core
                  opencv_highgui
                  opencv_calib3d 
                  opencv_contrib
                  opencv_features2d 
                  opencv_flann )

add_executable ( proj5-1-2  ${SRC_FILES} )
target_link_libraries( proj5-1-2 ${OpenCV_LIBS} )
