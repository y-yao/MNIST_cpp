//========================================================================
// hrs.h
//========================================================================
// Declarations for handwriting recognition systems

#ifndef HRS_H
#define HRS_H

#include "types.h"
#include <vector>

//------------------------------------------------------------------------
// IHandwritingRecSys
//------------------------------------------------------------------------
// Abstract base class for handwriting recognition systems
//
// - train    : Train the handwriting recognition system with a vector of
//              labeled images
// - classify : Classify an image and return a label
//

class IHandwritingRecSys
{
 public:
  virtual void  train    ( const std::vector<LabeledImage>& v ) = 0;
  virtual Label classify ( const Image& image                 ) = 0;
};

#endif // HRS_H

