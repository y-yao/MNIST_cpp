#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>

#define UTST_BEGIN() \
  std::printf("------- Testing %s -------\n\n", __FILE__);

#define UTST_END() \
  std::printf("\n------- Finished testing %s -------\n", __FILE__);

#define UTST_TEST_LABEL( x ) \
  std::printf("\n>>> %s\n\n", (x));

#define UTST_ASSERT_TRUE( x ) \
  { \
    int x_value = (int) (x); \
    if ( !x_value ) { \
      std::printf("\tLine %d: [ \033[31m FAILED \033[0m ]\n", __LINE__); \
      return 1; \
    } else { \
      std::printf("\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__); \
    } \
  }

#define UTST_ASSERT_FALSE( x ) \
  { \
    int x_value = (int) (x); \
    if ( x_value ) { \
      std::printf("\tLine %d: [ \033[31m FAILED \033[0m ]\n", __LINE__); \
      return 1; \
    } else { \
      std::printf("\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__); \
    } \
  }

#define UTST_ASSERT_INT_EQ( x, y ) \
  { \
    int x_value = (int) (x); \
    int y_value = (int) (y); \
    if ( x_value != y_value ) { \
      std::printf("\tLine %d: [ \033[31m FAILED \033[0m ] %d != %d\n", __LINE__, x_value, y_value); \
      return 1; \
    } else { \
      std::printf("\tLine %d: [ \033[32m passed \033[0m ] %d == %d\n", __LINE__, x_value, y_value); \
    } \
  }

#define UTST_ASSERT_FLOAT_EQ( x, y, eps ) \
  { \
    float x_value = (float) (x); \
    float y_value = (float) (y); \
    float eps_value = (float) (eps); \
    if ( std::fabs( x_value - y_value ) > eps_value ) { \
      std::printf("\tLine %d: [ \033[31m FAILED \033[0m ] %f != %f\n", __LINE__, x_value, y_value); \
      return 1; \
    } else { \
      std::printf("\tLine %d: [ \033[32m passed \033[0m ] %f == %f\n", __LINE__, x_value, y_value); \
    } \
  }

#define UTST_ASSERT_DOUBLE_EQ( x, y, eps ) \
  { \
    double x_value = (double) (x); \
    double y_value = (double) (y); \
    double eps_value = (double) (eps); \
    if ( std::abs( x_value - y_value ) > eps_value ) { \
      std::printf("\tLine %d: [ \033[31m FAILED \033[0m ] %lf != %f\n", __LINE__, x_value, y_value); \
      return 1; \
    } else { \
      std::printf("\tLine %d: [ \033[32m passed \033[0m ] %lf == %f\n", __LINE__, x_value, y_value); \
    } \
  }

#define UTST_ASSERT_CHAR_EQ( x, y ) \
  { \
    char x_value = (char) (x); \
    char y_value = (char) (y); \
    if ( x_value != y_value ) { \
      std::printf("\tLine %d: [ \033[31m FAILED \033[0m ] %c != %c\n", __LINE__, x_value, y_value); \
      return 1; \
    } else { \
      std::printf("\tLine %d: [ \033[32m passed \033[0m ] %c == %c\n", __LINE__, x_value, y_value); \
    } \
  }

#define UTST_ASSERT_THROW( func, exception ) \
  { \
    try { \
      func; \
    } catch ( exception e ) { \
      UTST_ASSERT_TRUE( 1 );  \
    } \
  }

#endif
