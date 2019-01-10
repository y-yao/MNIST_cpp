//========================================================================
// hrs-knn-kdtree-test.cc
//========================================================================
// This file contains the tests for the handwriting recognition system

#include <iostream>
#include "hrs.h"
#include "hrs-knn-kdtree.h"
#include "constants.h"
#include "types.h"
#include "utils.h"
#include "utst.h"

//========================================================================
// Inputs
//========================================================================

#include "digits.dat"

int main() {

  UTST_BEGIN();

  //========================================================================
  // Basic Tests
  //========================================================================
  // These are simple test cases to check that the most basic functionality
  // works correctly.
  //
  // Do not add any additional tests here.

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                           STAFF NOTE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Many tests have been provided for you that test the handwriting
  // recognition system's train and classify interface.
  //
  // Make sure to test your k-d tree and any other supporting classes,
  // either here or in another -test.cc file!
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //------------------------------------------------------------------------
  // Digit10
  //------------------------------------------------------------------------
  // Classify digit10 with a training set of 5 images

  UTST_TEST_LABEL( "Digit10 should predict correctly with a training set of 5 images" );

  {
    // Create the LabeledImage from the .dat image

    Image image( digit10_image );
    Label label( digit10_label );

    // Train with the first 5 MNIST training images

    const size_t size_training = 5;
    std::vector<LabeledImage> v_training;
    v_training.reserve( size_training );

    read_labeled_images( "tiny-training-images-10.bin",  // image binary
                         "tiny-training-labels-10.bin",  // label binary
                         &v_training,                    // vector to fill
                         size_training );                // how many to read

    // Train and classify this digit

    HandwritingRecSysKnnKdTree hrs;                      // Declare the hrs
    hrs.train( v_training );                             // Train
    Label p = hrs.classify( image );                     // Classify

    // Uncomment these prints to see the training images

    // for ( auto& item : v_training )
    //   item.print();
    //
    // image.print(); // classification image

    // Check if prediction is '10'
    UTST_ASSERT_CHAR_EQ( p.get_label(), digit10_label );
  }

  //------------------------------------------------------------------------
  // Digit9
  //------------------------------------------------------------------------
  // Classify digit9 with a training set of 20 images

  UTST_TEST_LABEL("Digit9 should predict correctly with a training set of 20 images");

  {
    // Create the LabeledImage from the .dat image

    Image image( digit9_image );
    Label label( digit9_label );

    // Train with the first 20 MNIST training images

    const size_t size_training = 20;
    std::vector<LabeledImage> v_training;
    v_training.reserve( size_training );

    read_labeled_images( "small-training-images-1000.bin",  // image binary
                         "small-training-labels-1000.bin",  // label binary
                         &v_training,                       // vector to fill
                         size_training );                   // how many to read

    // Train and classify this digit

    HandwritingRecSysKnnKdTree hrs;                         // Declare the hrs
    hrs.train( v_training );                                // Train
    Label p = hrs.classify( image );                        // Classify

    // Uncomment these prints to see the training images

    // for ( auto& item : v_training )
    //   item.print();
    //
    // image.print(); // classification image

    // Check if prediction is '9'
    UTST_ASSERT_CHAR_EQ( p.get_label(), digit9_label );
  }

  //========================================================================
  // Larger Tests
  //========================================================================

  //------------------------------------------------------------------------
  // all_digits
  //------------------------------------------------------------------------

  // Test all digits after training with 1000 images

  UTST_TEST_LABEL("Test all digits after training with 1000 images");

  {
    // Train with the first 1000 MNIST images

    const size_t size_training = 1000;
    std::vector<LabeledImage> v_training;
    v_training.reserve( size_training );

    read_labeled_images( "small-training-images-1000.bin",  // image binary
                         "small-training-labels-1000.bin",  // label binary
                         &v_training,                       // vector to fill
                         size_training );                   // how many to read

    // Train the system

    HandwritingRecSysKnnKdTree hrs;
    hrs.train( v_training );

    // Get the digits from the .dat file

    Image image0  ( digit0_image  );
    Image image1  ( digit1_image  );
    Image image2  ( digit2_image  );
    Image image3  ( digit3_image  );
    Image image4  ( digit4_image  );
    Image image5  ( digit5_image  );
    Image image6  ( digit6_image  );
    Image image7  ( digit7_image  );
    Image image8  ( digit8_image  );
    Image image9  ( digit9_image  );
    Image image10 ( digit10_image );
    Image image11 ( digit11_image );
    Image image12 ( digit12_image );
    Image image13 ( digit13_image );

    // Classify and check predictions (all should predict correctly)

    Label p0  = hrs.classify( image0 );
    UTST_ASSERT_CHAR_EQ( p0.get_label(), digit0_label );

    Label p1  = hrs.classify( image1 );
    UTST_ASSERT_CHAR_EQ( p1.get_label(), digit1_label );

    Label p2  = hrs.classify( image2 );
    UTST_ASSERT_CHAR_EQ( p2.get_label(), digit2_label );

    Label p3  = hrs.classify( image3 );
    UTST_ASSERT_CHAR_EQ( p3.get_label(), digit3_label );

    Label p4  = hrs.classify( image4 );
    UTST_ASSERT_CHAR_EQ( p4.get_label(), digit4_label );

    Label p5  = hrs.classify( image5 );
    UTST_ASSERT_CHAR_EQ( p5.get_label(), digit5_label );

    Label p6  = hrs.classify( image6 );
    UTST_ASSERT_CHAR_EQ( p6.get_label(), digit6_label );

    Label p7  = hrs.classify( image7 );
    UTST_ASSERT_CHAR_EQ( p7.get_label(), digit7_label );

    Label p8  = hrs.classify( image8 );
    UTST_ASSERT_CHAR_EQ( p8.get_label(), digit8_label );

    Label p9  = hrs.classify( image9 );
    UTST_ASSERT_CHAR_EQ( p9.get_label(), digit9_label );

    Label p10 = hrs.classify( image10 );
    UTST_ASSERT_CHAR_EQ( p10.get_label(), digit10_label );

    Label p11 = hrs.classify( image11 );
    UTST_ASSERT_CHAR_EQ( p11.get_label(), digit11_label );

    Label p12 = hrs.classify( image12 );
    UTST_ASSERT_CHAR_EQ( p12.get_label(), digit12_label );

    Label p13 = hrs.classify( image13 );
    UTST_ASSERT_CHAR_EQ( p13.get_label(), digit13_label );
  }

  //------------------------------------------------------------------------
  // mnist_t10_i10
  //------------------------------------------------------------------------

  UTST_TEST_LABEL("Test with mnist_t10_i10 dataset");

  {
    const size_t size_training  = 10;
    const size_t size_inference = 10;

    // Read images and labels for the training dataset from the binaries

    std::vector<LabeledImage> v_training;
    v_training.reserve( size_training );

    read_labeled_images( "tiny-training-images-10.bin", // image binary
                         "tiny-training-labels-10.bin", // label binary
                         &v_training,                   // vector to fill
                         size_training );               // how many to read

    // Read images and labels for the inference dataset from the binaries

    std::vector<LabeledImage> v_inference;
    v_inference.reserve( size_inference );

    read_labeled_images( "tiny-inference-images-10.bin", // image binary
                         "tiny-inference-labels-10.bin", // label binary
                         &v_inference,                   // vector to fill
                         size_inference );               // how many to read

    // Train and classify using this handwriting recognition system

    HandwritingRecSysKnnKdTree hrs;

    double accuracy =
      train_and_classify( hrs, v_training, v_inference );

    std::cout << "Accuracy (kdtree, mnist_t10_i10): ";
    std::cout << accuracy << std::endl;

    // We don't know what the accuracy should be... so we don't call any
    // UTST_ASSERT macros
  }

  //------------------------------------------------------------------------
  // mnist_t600_i100 (with n_neighbors = 1)
  //------------------------------------------------------------------------

  UTST_TEST_LABEL("Test with mnist_t600_i100 dataset (with n_neighbors = 1)");

  {
    const size_t size_training  = 600;
    const size_t size_inference = 100;

    // Read images and labels for the training dataset from the binaries

    std::vector<LabeledImage> v_training;
    v_training.reserve( size_training );

    read_labeled_images( "small-training-images-1000.bin",  // image binary
                         "small-training-labels-1000.bin",  // label binary
                         &v_training,                       // vector to fill
                         size_training );                   // how many to read

    // Read images and labels for the inference dataset from the binaries

    std::vector<LabeledImage> v_inference;
    v_inference.reserve( size_inference );

    read_labeled_images( "small-inference-images-1000.bin", // image binary
                         "small-inference-labels-1000.bin", // label binary
                         &v_inference,                      // vector to fill
                         size_inference );                  // how many to read

    // Train and classify using this handwriting recognition system

    HandwritingRecSysKnnKdTree hrs;

    double accuracy =
      train_and_classify( hrs, v_training, v_inference );

    std::cout << "Accuracy (kdtree, mnist_t600_i100): ";
    std::cout << accuracy << std::endl;

    // We don't know what the accuracy should be... so we don't call any
    // UTST_ASSERT macros
  }

  UTST_END();

}
