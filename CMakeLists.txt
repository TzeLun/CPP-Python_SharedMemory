#MIT License
#
#Copyright (c) 2023 TzeLun
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.
#
# Author: Tze Lun, Lok
# Date : February 6, 2023
#

cmake_minimum_required(VERSION 3.20)
project(shm_test)

set(CMAKE_CXX_STANDARD 14)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} shm_test.cpp shmpy.py interprocess.h)

# include the directories to important libraries
target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${Python3_INCLUDE_DIRS}
)

# link to libraries at compile time
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${Python3_LIBRARIES}
            ${PYTHON_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE librt.so
)
