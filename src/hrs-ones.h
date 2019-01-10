//========================================================================
// hrs-ones.h
//========================================================================
// Declarations for handwriting recognition systems

#ifndef HRS_ONES_H
#define HRS_ONES_H

#include "types.h"
#include "hrs.h"
#include <vector>

//------------------------------------------------------------------------
// HandwritingRecSysClassifyOnes
//------------------------------------------------------------------------
// This handwriting recognition system detects ones with an algorithm and
// does not require training.

class HandwritingRecSysClassifyOnes : public IHandwritingRecSys
{
 public:
  void  train    ( const std::vector<LabeledImage>& v );
  Label classify ( const Image& image );

 private:
  Label detect_ones( const Image& image );

  // This function fit all non zero pixels with a straight line
  // and returns the mean squared error of the fit.
  // The input are x and y coordinates of all non-zero pixels
  double linear_regression( 
  const std::vector<size_t>& x_coords,
  const std::vector<size_t> y_coords ) const;
 
};

#endif // HRS_ONES_H

