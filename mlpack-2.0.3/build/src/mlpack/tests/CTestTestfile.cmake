# CMake generated Testfile for 
# Source directory: /home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/src/mlpack/tests
# Build directory: /home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/src/mlpack/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(mlpack_test "/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/bin/mlpack_test" "--log_level=test_suite")
SET_TESTS_PROPERTIES(mlpack_test PROPERTIES  WORKING_DIRECTORY "/home/enrico-giordano/TesiMagistrale/mlpack-2.0.3/build/")
