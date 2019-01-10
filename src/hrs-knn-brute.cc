//========================================================================
// hrs-knn-brute.cc
//========================================================================
// Handwriting recognition system member function definitions

#include "hrs-knn-brute.h"
#include <array>
#include <cassert>
#include <cmath>

//========================================================================
// HandwritingRecSysKnnBruteForce
//========================================================================
//------------------------------------------------------------------------
// train
//------------------------------------------------------------------------

void HandwritingRecSysKnnBruteForce::train( const std::vector<LabeledImage>& v )
{
  m_training_set = v;
}

//------------------------------------------------------------------------
// classify
//------------------------------------------------------------------------

Label HandwritingRecSysKnnBruteForce::classify( const Image& image )
{
  return knn( image );
}

//----------------------------------------------------------------------
// knn
//----------------------------------------------------------------------

Label HandwritingRecSysKnnBruteForce::knn( const Image& image )
{
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''
  // Implement this function
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  unsigned min_sqr_dist = std::pow(255, 2) * 784;  // Initialize the minimum squared distance with the
                                                   // maximum possible value
  char label = Label::INVALID;                                      // label record the current closest label.

  for ( const auto& m_training_sample : m_training_set ) {
    unsigned sqr_dist = calculate_sqr_dist(m_training_sample.get_image(), image, min_sqr_dist); // calculate squared distance
    if ( sqr_dist < min_sqr_dist ) {            // if the current squared distance is smaller than the last one
      min_sqr_dist = sqr_dist;                  // update the squared distance
      label = m_training_sample.get_label();    // update the label 
    }
  } 
  return Label(label);                      // return the closest label.
}

// This function calculates the squared distance between two images
// The input are two image objects, and the squared distance is returned
unsigned HandwritingRecSysKnnBruteForce::calculate_sqr_dist( const Image& img0, const Image& img1, double current_min ) const 
{
  unsigned sqr_dist = 0;                // Initialize squared distance
  Image::ConstItr itr0 = img0.begin();  // I nitialize two iterators as the first pixel
  Image::ConstItr itr1 = img1.begin();
  while ( itr0 != img0.end() ) {        // loop through all corresponding pixels
    sqr_dist += std::pow( (int)*itr0 - (int)*itr1, 2);  // Calculate squared difference and add to the sum
    if ( sqr_dist > current_min ) {
      return sqr_dist;
    }
    ++itr0;
    ++itr1;    
  } 
  return sqr_dist;
}

