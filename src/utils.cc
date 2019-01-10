//========================================================================
// utils.cc
//========================================================================
// Useful functions

#include "utils.h"
#include "hrs-ones.h"
#include <fstream>
#include <typeinfo>

//------------------------------------------------------------------------
// read_labeled_images
//------------------------------------------------------------------------

void read_labeled_images( const std::string&               fname_images,
                          const std::string&               fname_labels,
                                std::vector<LabeledImage>* labeled_images,
                                unsigned int               size )
{
  std::ifstream myifs;

  unsigned int n_misc_in_image_file = 4; // MNIST has 4 misc values in image bin
  unsigned int n_misc_in_label_file = 2; // MNIST has 2 misc values in label bin

  //----------------------------------------------------------------------
  // Read images
  //----------------------------------------------------------------------

  std::vector<Image> images;
  images.reserve( size );

  // Open binary file

  myifs.open( fname_images, std::ios::in | std::ios::binary );

  // Grab the miscellaneous front matter (4B each) and throw it away.
  //
  // Note that because the endianness is flipped, in order to print these
  // values out, we would have to flip the endianness using endian_swap()
  // before printing.

  unsigned int misc = 0;
  for ( unsigned int i = 0; i < n_misc_in_image_file; i++ )
    myifs.read( (char *) &misc, 4 );

  // Grab each image (28 x 28 bytes) from the file and add to our
  // container

  for ( size_t idx = 0; idx < size; ++idx ) {

    // Read the data from the file

    std::array<unsigned int, image_size> data;

    for ( size_t i = 0; i < n_rows; ++i ) {
      for ( size_t j = 0; j < n_cols; ++j ) {
        unsigned int tmp = 0;
        myifs.read( (char *) &tmp, 1 );
        data[ i*28 + j ] = tmp;
      }
    }

    // Push this image into the container

    images.push_back( Image(data) );
  }

  // Close file

  myifs.close();

  //----------------------------------------------------------------------
  // Read labels
  //----------------------------------------------------------------------

  std::vector<Label> labels;
  labels.reserve( size );

  // Open binary file

  myifs.open( fname_labels, std::ios::in | std::ios::binary );

  // Grab the miscellaneous front matter (4B each) and throw it away.
  //
  // Note that because the endianness is flipped, in order to print these
  // values out, we would have to flip the endianness using endian_swap()
  // before printing.

  for ( unsigned int i = 0; i < n_misc_in_label_file; i++ )
    myifs.read( (char *) &misc, 4 );

  // Grab each label and label the corresponding image

  for ( size_t i = 0; i < size; ++i ) {

    // Read the label from the file (and convert digit to char)

    unsigned int label_uint = 0;
    myifs.read( (char *) &label_uint, 1 );
    char label_char = '0' + label_uint;

    // Push this label into the container

    labels.push_back( Label(label_char) );
  }

  //----------------------------------------------------------------------
  // Create LabeledImage vector
  //----------------------------------------------------------------------

  for ( size_t i = 0; i < size; ++i )
    labeled_images->push_back( LabeledImage( images[i], labels[i] ) );

  // Close file

  myifs.close();
}

//------------------------------------------------------------------------
// train_and_classify
//------------------------------------------------------------------------
// Takes a handwriting recognition system, trains it with all of the
// labeled images in the training set, classifies all of the images in the
// inference set, and then checks how many classifications are correct,
// returning the accuracy as a double.

double train_and_classify( IHandwritingRecSys& hrs,
                           std::vector<LabeledImage>& v_training,
                           std::vector<LabeledImage>& v_inference )
{
  // Return if there are no images to classify inside the inference set

  if ( v_inference.empty() ) return 0;

  // Training

  hrs.train( v_training );

  // Inference

  unsigned int num_correct = 0;
  unsigned int idx         = 0;

  for ( const auto& labeled_image : v_inference ) {
    Label prediction = hrs.classify( labeled_image.get_image() );

    if ( prediction.get_label() == labeled_image.get_label() )
      ++num_correct;

    // Progress

    if ( ++idx % 1000 == 0 ) {
      std::cout << "Classified index              : " << idx         << std::endl;
      std::cout << "- Num correct classifications : " << num_correct << std::endl;
    }
  }

  // Check accuracy

  double accuracy = 1.0 * num_correct / v_inference.size();

  return accuracy;
}

