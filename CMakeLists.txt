cmake_minimum_required (VERSION 3.10)
project (MFQuery VERSION 0.8 LANGUAGES CXX)

#specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/MFQueryInputParser/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/MFQueryInputParser/src)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/MFQueryInputParser/bin)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${INCLUDE_DIR}")

aux_source_directory(${SRC_DIR} DIR_SRCS)
# add the executable
add_executable(main ${DIR_SRCS})

install(TARGETS main DESTINATION ${BIN_DIR})
