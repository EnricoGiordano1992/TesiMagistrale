# Install script for directory: /home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/src/mlpack/core/optimizers

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/adadelta/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/adam/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/aug_lagrangian/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/lbfgs/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/minibatch_sgd/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/rmsprop/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/sa/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/sdp/cmake_install.cmake")
  INCLUDE("/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/core/optimizers/sgd/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)
