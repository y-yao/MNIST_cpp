//========================================================================
// types-test.cc
//========================================================================
// This file contains the tests for the types

#include <iostream>
#include "types.h"
#include "utst.h"

//------------------------------------------------------------------------
// Dataset
//------------------------------------------------------------------------

#include "digits.dat"

int main() {

  UTST_BEGIN();

  //------------------------------------------------------------------------
  // Basic Tests
  //------------------------------------------------------------------------
  // These are simple test cases to check that the most basic functionality
  // works correctly.
  //
  // Do not add any additional tests here.

  UTST_TEST_LABEL( "Image: Basic constructor and destructor" );

  {
    Image foo( digit0_image );
  }

  UTST_TEST_LABEL( "Image: Basic index operator overload" );

  {
    Image foo( digit0_image );

    // Check some data from sixth row

    UTST_ASSERT_INT_EQ( foo[ 5*28 + 12 ],   3 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 13 ],  18 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 14 ],  18 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 15 ],  18 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 16 ], 126 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 17 ], 136 );
  }

  UTST_TEST_LABEL( "Image: Basic at" );

  {
    Image foo( digit0_image );

    // Check some data from sixth row

    UTST_ASSERT_INT_EQ( foo.at( 12, 5 ),   3 );
    UTST_ASSERT_INT_EQ( foo.at( 13, 5 ),  18 );
    UTST_ASSERT_INT_EQ( foo.at( 14, 5 ),  18 );
    UTST_ASSERT_INT_EQ( foo.at( 15, 5 ),  18 );
    UTST_ASSERT_INT_EQ( foo.at( 16, 5 ), 126 );
    UTST_ASSERT_INT_EQ( foo.at( 17, 5 ), 136 );
  }

  UTST_TEST_LABEL( "Image: Basic iterator usage" );

  {
    Image foo( digit0_image );

    // Check that iterating from begin() to end() takes foo.size() steps

    size_t i = 0;

    for ( auto itr = foo.begin(); itr != foo.end(); ++itr )
      ++i;

    UTST_ASSERT_INT_EQ( i, foo.size() );
  }

  UTST_TEST_LABEL( "Image: Basic size" );

  {
    Image foo( digit0_image );

    UTST_ASSERT_INT_EQ( digit0_image.size(), foo.size() );
  }

  UTST_TEST_LABEL( "Label: Basic constructor and destructor" );

  {
    Label foo( 'e' );
  }

  UTST_TEST_LABEL( "Label: Basic setting and getting label" );

  {
    Label foo( 'e' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'e' );
    foo.set_label( 'x' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'x' );
  }

  UTST_TEST_LABEL( "LabeledImage: Basic constructor and destructor" );

  {
    Image         image( digit0_image );
    Label         label( 'e' );
    LabeledImage( image, label );
  }

  UTST_TEST_LABEL( "LabeledImage: Basic get label" );

  {
    Image        image( digit0_image );
    Label        label( 'e' );
    LabeledImage foo( image, label );

    UTST_ASSERT_CHAR_EQ( foo.get_label(), label.get_label() );
  }

  UTST_TEST_LABEL( "LabeledImage: Basic get image" );

  {
    Image        image( digit0_image );
    Label        label( 'e' );
    LabeledImage foo( image, label );

    const Image& foo_img = foo.get_image();

    // Check the entire image for equality against the original

    for ( size_t i = 0; i < digit0_image.size(); ++i )
      UTST_ASSERT_INT_EQ( foo_img[i], digit0_image[i] );
  }

  UTST_TEST_LABEL( "LabeledImage: Basic index operator overload" );

  {
    Image        image( digit0_image );
    Label        label( 'e' );
    LabeledImage foo( image, label );

    // Check some data from sixth row

    UTST_ASSERT_INT_EQ( foo[ 5*28 + 12 ],   3 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 13 ],  18 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 14 ],  18 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 15 ],  18 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 16 ], 126 );
    UTST_ASSERT_INT_EQ( foo[ 5*28 + 17 ], 136 );
  }

  //------------------------------------------------------------------------
  // Directed Tests
  //------------------------------------------------------------------------

  UTST_TEST_LABEL( "Image: Check the entire image against the data in the original std::array" );

  {
    Image foo( digit0_image );

    // Check the entire image

    for ( size_t i = 0; i < digit0_image.size(); ++i )
      UTST_ASSERT_INT_EQ( foo[i], digit0_image[i] );
  }

  UTST_TEST_LABEL( "Image: Check the entire image against the original data using iterators" );

  {
    Image foo( digit0_image );

    size_t i = 0;
    for ( auto itr = foo.begin(); itr != foo.end(); ++itr ) {
      UTST_ASSERT_INT_EQ( *itr, digit0_image[i] );
      ++i;
    }
  }

  UTST_TEST_LABEL( "Image: Directed at that should throw exception for out of range" );

  {
    Image foo( digit0_image );

    // Check for exception throw when accessing at x = 999

    UTST_ASSERT_THROW( foo.at( 999,   5 ), std::out_of_range );
    UTST_ASSERT_THROW( foo.at(   5, 999 ), std::out_of_range );
    UTST_ASSERT_THROW( foo.at( 999, 999 ), std::out_of_range );
  }

  UTST_TEST_LABEL( "Label: Get and set the alphanumeric chars" );

  {
    Label foo( '0' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '0' );
    foo.set_label(                 '1' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '1' );
    foo.set_label(                 '2' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '2' );
    foo.set_label(                 '3' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '3' );
    foo.set_label(                 '4' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '4' );
    foo.set_label(                 '5' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '5' );
    foo.set_label(                 '6' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '6' );
    foo.set_label(                 '7' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '7' );
    foo.set_label(                 '8' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '8' );
    foo.set_label(                 '9' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), '9' );

    foo.set_label(                 'a' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'a' );
    foo.set_label(                 'b' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'b' );
    foo.set_label(                 'c' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'c' );
    foo.set_label(                 'd' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'd' );
    foo.set_label(                 'e' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'e' );
    foo.set_label(                 'f' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'f' );
    foo.set_label(                 'g' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'g' );
    foo.set_label(                 'h' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'h' );
    foo.set_label(                 'i' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'i' );
    foo.set_label(                 'j' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'j' );
    foo.set_label(                 'k' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'k' );
    foo.set_label(                 'l' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'l' );
    foo.set_label(                 'm' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'm' );
    foo.set_label(                 'n' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'n' );
    foo.set_label(                 'o' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'o' );
    foo.set_label(                 'p' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'p' );
    foo.set_label(                 'q' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'q' );
    foo.set_label(                 'r' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'r' );
    foo.set_label(                 's' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 's' );
    foo.set_label(                 't' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 't' );
    foo.set_label(                 'u' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'u' );
    foo.set_label(                 'v' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'v' );
    foo.set_label(                 'w' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'w' );
    foo.set_label(                 'x' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'x' );
    foo.set_label(                 'y' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'y' );
    foo.set_label(                 'z' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'z' );

    foo.set_label(                 'A' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'A' );
    foo.set_label(                 'B' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'B' );
    foo.set_label(                 'C' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'C' );
    foo.set_label(                 'D' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'D' );
    foo.set_label(                 'E' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'E' );
    foo.set_label(                 'F' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'F' );
    foo.set_label(                 'G' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'G' );
    foo.set_label(                 'H' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'H' );
    foo.set_label(                 'I' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'I' );
    foo.set_label(                 'J' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'J' );
    foo.set_label(                 'K' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'K' );
    foo.set_label(                 'L' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'L' );
    foo.set_label(                 'M' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'M' );
    foo.set_label(                 'N' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'N' );
    foo.set_label(                 'O' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'O' );
    foo.set_label(                 'P' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'P' );
    foo.set_label(                 'Q' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'Q' );
    foo.set_label(                 'R' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'R' );
    foo.set_label(                 'S' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'S' );
    foo.set_label(                 'T' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'T' );
    foo.set_label(                 'U' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'U' );
    foo.set_label(                 'V' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'V' );
    foo.set_label(                 'W' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'W' );
    foo.set_label(                 'X' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'X' );
    foo.set_label(                 'Y' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'Y' );
    foo.set_label(                 'Z' );
    UTST_ASSERT_CHAR_EQ( foo.get_label(), 'Z' );
  }

  UTST_TEST_LABEL( "LabeledImage: Check the entire image against the original data using" );
  // iterators

  {
    Image        image( digit0_image );
    Label        label( 'x' );
    LabeledImage foo( image, label );

    size_t i = 0;
    for ( auto itr = foo.begin(); itr != foo.end(); ++itr ) {
      UTST_ASSERT_INT_EQ( *itr, digit0_image[i] );
      ++i;
    }
  }

  UTST_TEST_LABEL( "LabeledImage: Directed at that should throw exception for out of range" );

  {
    Image        image( digit0_image );
    Label        label( 'x' );
    LabeledImage foo( image, label );

    // Check for exception throw when accessing at x = 999

    UTST_ASSERT_THROW( foo.at( 999,   5 ), std::out_of_range );
    UTST_ASSERT_THROW( foo.at(   5, 999 ), std::out_of_range );
    UTST_ASSERT_THROW( foo.at( 999, 999 ), std::out_of_range );
  }

  UTST_END();
}
