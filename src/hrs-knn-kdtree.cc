//========================================================================
// hrs-knn-kdtree.cc
//========================================================================
// Handwriting recognition system member function definitions

#include "hrs-knn-kdtree.h"
#include <array>
#include <cassert>
#include <cmath>

//========================================================================
// HandwritingRecSysKnnKdTree
//========================================================================
//------------------------------------------------------------------------
// train
//------------------------------------------------------------------------

void HandwritingRecSysKnnKdTree::train( const std::vector<LabeledImage>& v )
{
  // Save the training data

  m_training_set = v;

  // Build the kdtree from the training data

  m_kdtree.build( m_training_set.begin(), m_training_set.end() );
}

//------------------------------------------------------------------------
// classify
//------------------------------------------------------------------------

Label HandwritingRecSysKnnKdTree::classify( const Image& image )
{
  // This handwriting recognition system classifies images with two steps:
  //
  // 1. Use the k-d tree to "zoom in" on a small partition with very few
  //    points.
  //
  // 2. Run a brute force knn on this small subset to find the nearest
  //    neighbor.

  // Step 1. Search the tree with the input image. We also declare a
  // "v_neighbors" vector and pass it into the k-d tree search. The k-d
  // tree will fill the vector with a small subset of potential neighbors.

  // Note: The k-d tree is built to search for labeled images and not
  // regular unlabeled images. We create a dummy labeled image from the
  // regular image and use it to walk the k-d tree.

  LabeledImage              dummy_labeled_image( image, Label::INVALID );
  std::vector<LabeledImage> v_neighbors;

  m_kdtree.search( dummy_labeled_image, v_neighbors );

  // Step 2. Now do a brute force search on the "v_neighbors" vector.

  return knn( image, v_neighbors );
}

//----------------------------------------------------------------------
// knn
//----------------------------------------------------------------------
// Brute force k-nearest neighbors across the given vector of labeled
// images. Returns the label of the exact nearest neighbor.

Label HandwritingRecSysKnnKdTree::knn( const Image& image,
                         std::vector<LabeledImage>& v_neighbors )
{
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''
  // This function executes brute-force k-nearest neighbors just as in the
  // HandwritingRecSysKnnBruteForce class. However, we no longer need to
  // search the entire training set "m_training_set" because the k-d tree
  // has provided a much smaller subset of nearby LabeledImage's.
  //
  // Copy your brute-force k-nearest neighbors solution and make light
  // adaptations so that it searches over the input vector "v_neighbors"
  // instead of over the entire "m_training_set".
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  unsigned min_sqr_dist = std::pow(255, 2) * 784;  // Initialize the minimum squared distance with the
                                                   // maximum possible value
  char label = '0';                                      // label record the current closest label.

  for ( const auto& m_training_sample : v_neighbors ) {
    unsigned sqr_dist = calculate_sqr_dist(m_training_sample.get_image(), image); // calculate squared distance
    if ( sqr_dist < min_sqr_dist ) {            // if the current squared distance is smaller than the last one
      min_sqr_dist = sqr_dist;                  // update the squared distance
      label = m_training_sample.get_label();    // update the label 
    }
  } 
  return Label(label);                      // return the closest label.

  return Label();
}


// This function calculates the squared distance between two images
// The input are two image objects, and the squared distance is returned
unsigned HandwritingRecSysKnnKdTree::calculate_sqr_dist( const Image& img0, const Image& img1 ) const 
{
  unsigned sqr_dist = 0;                // Initialize squared distance
  Image::ConstItr itr0 = img0.begin();  // I nitialize two iterators as the first pixel
  Image::ConstItr itr1 = img1.begin();
  while ( itr0 != img0.end() ) {        // loop through all corresponding pixels
    sqr_dist += std::pow( (int)*itr0 - (int)*itr1, 2);  // Calculate squared difference and add to the sum
    ++itr0;
    ++itr1;    
  } 
  return sqr_dist;
}

