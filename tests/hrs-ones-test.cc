//========================================================================
// hrs-ones-test.cc
//========================================================================
// This file contains the tests for the handwriting recognition system

#include <iostream>
#include "hrs.h"
#include "hrs-ones.h"
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
  // A few basic tests have been provided for you that test your classifier
  // on some digits in "digits.dat".
  //
  // There is no need to add further tests for this simple handwriting
  // recognition system since it only recognizes ones and has very limited
  // use. Just make sure you can pass these basic tests!
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //------------------------------------------------------------------------
  // Digit3
  //------------------------------------------------------------------------
  // Digit3 is a '1' and should predict correctly

  UTST_TEST_LABEL( "Digit3 is a '1' and should predict correctly" );

  {
    Image image( digit3_image );                 // Get the Image

    HandwritingRecSysClassifyOnes hrs;           // Declare the system
                                                 // No training for this hrs
    Label prediction = hrs.classify( image );    // Classify the image

    // Uncomment this print to see the image

    // image.print();

    // Check if prediction is '1'
    UTST_ASSERT_CHAR_EQ( prediction.get_label(), '1' );
  }

  //------------------------------------------------------------------------
  // Digit4
  //------------------------------------------------------------------------
  // Digit4 is a '1' and should predict correctly

  UTST_TEST_LABEL( "Digit4 is a '1' and should predict correctly" );

  {
    Image image( digit4_image );                 // Get the Image

    HandwritingRecSysClassifyOnes hrs;           // Declare the system
                                                 // No training for this hrs
    Label prediction = hrs.classify( image );    // Classify the image

    // Uncomment this print to see the image

    // image.print();

    // Check if prediction is '1'
    UTST_ASSERT_CHAR_EQ( prediction.get_label(), '1' );
  }

  //------------------------------------------------------------------------
  // Digit5
  //------------------------------------------------------------------------
  // Digit5 is not a '1' and should predict '0'

  UTST_TEST_LABEL( "Digit5 is not a '1' and should predict '0'" );

  {
    Image image( digit5_image );                 // Get the Image

    HandwritingRecSysClassifyOnes hrs;           // Declare the system
                                                 // No training for this hrs
    Label prediction = hrs.classify( image );    // Classify the image

    // Uncomment this print to see the image

    // image.print();

    // Check if prediction is '0'
    UTST_ASSERT_CHAR_EQ( prediction.get_label(), '0' );
  }

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment these to test larger datasets when you are ready
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

  //========================================================================
  // Larger Tests
  //========================================================================

  //------------------------------------------------------------------------
  // all_digits
  //------------------------------------------------------------------------

  // Test all digits

  UTST_TEST_LABEL( "Test all digits" );

  {
    HandwritingRecSysClassifyOnes hrs;

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

    // Classify and check predictions
    //
    // Only digit3 and digit4 are '1'. All others digits are not '1' and
    // should be classified as '0'.

    Label p0  = hrs.classify( image0 );
    UTST_ASSERT_CHAR_EQ( p0.get_label(), '0' );

    Label p1  = hrs.classify( image1 );
    UTST_ASSERT_CHAR_EQ( p1.get_label(), '0' );

    Label p2  = hrs.classify( image2 );
    UTST_ASSERT_CHAR_EQ( p2.get_label(), '0' );

    Label p3  = hrs.classify( image3 );
    UTST_ASSERT_CHAR_EQ( p3.get_label(), digit3_label);

    Label p4  = hrs.classify( image4 );
    UTST_ASSERT_CHAR_EQ( p4.get_label(), digit4_label);

    Label p5  = hrs.classify( image5 );
    UTST_ASSERT_CHAR_EQ( p5.get_label(), '0' );

    Label p6  = hrs.classify( image6 );
    UTST_ASSERT_CHAR_EQ( p6.get_label(), '0' );

    Label p7  = hrs.classify( image7 );
    UTST_ASSERT_CHAR_EQ( p7.get_label(), '0' );

    Label p8  = hrs.classify( image8 );
    UTST_ASSERT_CHAR_EQ( p8.get_label(), '0' );

    Label p9  = hrs.classify( image9 );
    UTST_ASSERT_CHAR_EQ( p9.get_label(), '0' );

    Label p10 = hrs.classify( image10 );
    UTST_ASSERT_CHAR_EQ( p10.get_label(), '0' );

    Label p11 = hrs.classify( image11 );
    UTST_ASSERT_CHAR_EQ( p11.get_label(), '0' );

    Label p12 = hrs.classify( image12 );
    UTST_ASSERT_CHAR_EQ( p12.get_label(), '0' );

    Label p13 = hrs.classify( image13 );
    UTST_ASSERT_CHAR_EQ( p13.get_label(), '0' );
  }

  //------------------------------------------------------------------------
  // mnist_t10_i10
  //------------------------------------------------------------------------

  UTST_TEST_LABEL( " Test with mnist_t10_i10 dataset" );

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

    HandwritingRecSysClassifyOnes hrs;

    double accuracy =
      train_and_classify( hrs, v_training, v_inference );

    std::cout << "Accuracy (classify_ones, mnist_t10_i10): ";
    std::cout << accuracy << std::endl;

    // Accuracy should be...

    double accuracy_expected = 0.3;

    UTST_ASSERT_DOUBLE_EQ( accuracy / accuracy_expected, 1.0, accuracy_precision );
  }

  //------------------------------------------------------------------------
  // mnist_t600_i100
  //------------------------------------------------------------------------

  UTST_TEST_LABEL( "Test with mnist_t600_i100 dataset" );

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

    HandwritingRecSysClassifyOnes hrs;

    double accuracy =
      train_and_classify( hrs, v_training, v_inference );

    std::cout << "Accuracy (classify_ones, mnist_t600_i100): ";
    std::cout << accuracy << std::endl;

    // Accuracy should be...

    double accuracy_expected = 0.22;

    double accuracy_precision_adj = accuracy_precision * 10; // allow 10% swing

    UTST_ASSERT_DOUBLE_EQ( accuracy / accuracy_expected, 1.0, accuracy_precision_adj );
  }

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment when you are ready
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  
  UTST_END();

  return 0;
}
