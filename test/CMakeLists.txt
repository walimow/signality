
cmake_minimum_required(VERSION 2.6)

project(signality_test)

 
# Setup testing

enable_testing()

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
 

# Add test cpp file

add_executable(runUnitTests

   signality_test.cpp

)

 

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG} pthread ) #pthread i had to add (? -k.h.)

#target_link_libraries(runUnitTests ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

add_test(

    NAME runUnitTests

    COMMAND runUnitTests

)

 
