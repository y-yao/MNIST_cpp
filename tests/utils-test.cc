//========================================================================
// utils-test.cc
//========================================================================
// This file contains the tests for the misc utils

#include <iostream>
#include "utils.h"
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

  UTST_TEST_LABEL( "Read 1 image from the tiny training set of 10" );

  {
    std::vector<LabeledImage> v;

    // Read 1 image

    read_labeled_images( "tiny-training-images-10.bin",
                         "tiny-training-labels-10.bin",
                         &v, 1 );

    // The digit0 stored in the dat is the first image in the MNIST training
    // dataset. Check the image we just read against the version stored in
    // the dat.

    for ( size_t i = 0; i < image_size; ++i )
      UTST_ASSERT_INT_EQ( v[0][i], digit0_image[i] );
  }

  UTST_TEST_LABEL( "Read 10 images from the tiny training set of 10" );

  {
    std::vector<LabeledImage> v;

    // Read 10 images

    read_labeled_images( "tiny-training-images-10.bin",
                         "tiny-training-labels-10.bin",
                         &v, 10 );

    // The digit0 stored in the dat is the first image in the MNIST training
    // dataset. Check the image we just read against the version stored in
    // the dat.

    for ( size_t i = 0; i < image_size; ++i )
      UTST_ASSERT_INT_EQ( v[0][i], digit0_image[i] );
  }

  UTST_END();
}
