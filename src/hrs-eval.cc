//========================================================================
// hrs-eval.cc
//========================================================================
// This file evaluates the handwriting recognition systems

#include "hrs.h"
#include "hrs-ones.h"
#include "hrs-knn-brute.h"
#include "hrs-knn-kdtree.h"
#include "hrs-alt.h"
#include "constants.h"
#include "types.h"
#include "utils.h"

#include <iomanip> // for std::setw
#include <iostream>
#include <sys/time.h>
#include <typeinfo>

const char* cursor_u = "\033[A";
const char* cursor_e = "\033[K";

//========================================================================
// Input dataset
//========================================================================

#include "digits.dat"

//========================================================================
// Helper functions
//========================================================================

//------------------------------------------------------------------------
// classify
//------------------------------------------------------------------------

double classify( IHandwritingRecSys&              hrs,
                 std::string                      hrs_name,
                 const std::vector<LabeledImage>& v_inference )
{
  // Return if there are no images to classify inside the inference set

  if ( v_inference.empty() ) return 0;

  // Set up progress bar for inference

  size_t size          = v_inference.size();
  size_t n_fractions   = 70; // 70th
  size_t fraction_size;

  if ( size > n_fractions ) fraction_size = size / n_fractions;
  else                      fraction_size = 1;

  size_t len_progress_bar = size / fraction_size;

  // Run inference on each inference image

  size_t       idx         = 0;
  unsigned int num_correct = 0;

  for ( const auto& labeled_image : v_inference ) {

    // Classify

    Label prediction = hrs.classify( labeled_image.get_image() );

    if ( prediction.get_label() == labeled_image.get_label() )
      ++num_correct;

    // Progress bar

    if ( idx++ % fraction_size == 0 ) {
      size_t n_progress_markers = idx / fraction_size;

      std::cout << "  ";
      std::cout << std::setw(36) << std::left << hrs_name;
      std::cout << " | ";
      std::cout << std::endl;
      std::cout << std::endl;
      std::cout << "[ ";
      for ( size_t i = 0; i < n_progress_markers; ++i )
        std::cout << "=";
      for ( size_t i = 0; i < len_progress_bar - n_progress_markers; ++i )
        std::cout << ".";
      std::cout << " ]";
      std::cout << cursor_e;
      std::cout << std::endl;

      std::cout << "- Classifying index " << idx << " of " << size << "...";
      std::cout << cursor_e;
      std::cout << std::endl;
      std::cout << "- Num correct classifications : " << num_correct;
      std::cout << cursor_e;
      std::cout << std::endl;

      std::cout << cursor_u << cursor_u << cursor_u << cursor_u << cursor_u;
    }
  }

  // Clear any extra output due to cursor movement

  std::cout << cursor_e << std::endl;
  std::cout << cursor_e << std::endl;
  std::cout << cursor_e << std::endl;
  std::cout << cursor_e << std::endl;
  std::cout << cursor_e << std::endl;
  std::cout << cursor_u << cursor_u << cursor_u << cursor_u << cursor_u;

  // Check accuracy

  double accuracy = 1.0 * num_correct / size;

  return accuracy;
}

//------------------------------------------------------------------------
// time_hrs
//------------------------------------------------------------------------

void time_hrs( IHandwritingRecSys&              hrs,
               std::string                      hrs_name,
               const std::vector<LabeledImage>& v_training,
               const std::vector<LabeledImage>& v_inference )
{
  // Time the training phase of this handwriting recognition system

  struct timeval t_start;
  struct timeval t_end;

  std::cout << "  ";
  std::cout << std::setw(36) << std::left << hrs_name;
  std::cout << " | ";
  std::cout << "Training ..." << std::endl;

  gettimeofday( &t_start, NULL );                   // Timer on
  hrs.train( v_training );                          // Train
  gettimeofday( &t_end, NULL );                     // Timer off

  std::cout << cursor_u << cursor_e;

  double t_elapsed = ( t_end.tv_sec  - t_start.tv_sec ) +
                   ( ( t_end.tv_usec - t_start.tv_usec ) / 1000000.0 );

  // Time the inference phase using this handwriting recognition system

  struct timeval i_start;
  struct timeval i_end;

  gettimeofday( &i_start, NULL );                  // Timer on
  double accuracy = classify( hrs,                 // Run inference
                              hrs_name,
                              v_inference );
  gettimeofday( &i_end, NULL );                    // Timer off

  double i_elapsed = ( i_end.tv_sec  - i_start.tv_sec ) +
                   ( ( i_end.tv_usec - i_start.tv_usec ) / 1000000.0 );

  // Report results into a line of the table

  std::cout.precision(3);
  std::cout << "  ";
  std::cout << std::setw(36) << std::left << hrs_name;
  std::cout << " | ";
  std::cout << std::setw(8) << std::left << t_elapsed;
  std::cout << "  ";
  std::cout << std::setw(9) << std::left << i_elapsed;
  std::cout << "  ";
  std::cout << std::setw(4) << std::left << accuracy*100 << "%";
  std::cout << std::endl;
}

//========================================================================
// Main
//========================================================================

int main( int argc, char *argv[] )
{
  // This evaluation program takes a single argument, which is a parameter
  // that divides the training and inference dataset sizes so they are
  // smaller (e.g., divide by 2 or 4). If the argument is not provided,
  // the factor defaults to divide by 1 (i.e., full dataset sizes).

  int dataset_factor = 1;

  if ( argc == 2 ) {
    std::string arg1( argv[1] );        // Make a string from the argument
    dataset_factor = std::stoi( arg1 ); // Convert to an integer
  }

  const size_t size_training  = 60000 / dataset_factor; // MNIST training set
  const size_t size_inference = 10000 / dataset_factor; // MNIST inference set

  // Read images and labels for the training dataset from the binaries

  std::vector<LabeledImage> v_training;
  v_training.reserve( size_training );

  read_labeled_images( "training-images.bin",  // image binary
                       "training-labels.bin",  // label binary
                       &v_training,            // vector to fill
                       size_training );        // how many to read

  // Read images and labels for the inference dataset from the binaries

  std::vector<LabeledImage> v_inference;
  v_inference.reserve( size_inference );

  read_labeled_images( "inference-images.bin", // image binary
                       "inference-labels.bin", // label binary
                       &v_inference,           // vector to fill
                       size_inference );       // how many to read

  // Evaluation Table

  std::cout <<                                                                                 std::endl;
  std::cout << "  size_training  = " << size_training  << " images"                         << std::endl;
  std::cout << "  size_inference = " << size_inference << " images"                         << std::endl;
  std::cout << "                                         Training  Inference              " << std::endl;
  std::cout << "  System                                 Time (s)  Time (s)   Accuracy    " << std::endl;
  std::cout << "  -------------------------------------+----------------------------------" << std::endl;

  HandwritingRecSysClassifyOnes  hrs0;
  HandwritingRecSysKnnBruteForce hrs1;
  HandwritingRecSysKnnKdTree     hrs2;
  HandwritingRecSysAlt           hrs3;

  time_hrs( hrs0, "Classify Ones",                   v_training, v_inference );
  time_hrs( hrs1, "KNN Brute Force (neighbors = 1)", v_training, v_inference );
  time_hrs( hrs2, "KNN K-D Tree (neighbors = 1)",    v_training, v_inference );
  time_hrs( hrs3, "Alternative",                     v_training, v_inference );
}

