//========================================================================
// hrs-alt.h
//========================================================================
// Declarations for handwriting recognition systems

#ifndef HRS_ALT_H
#define HRS_ALT_H

#include "types.h"
#include "hrs.h"
#include <vector>

//------------------------------------------------------------------------
// HandwritingRecSysAlt
//------------------------------------------------------------------------
// Uses a different approach to achieve high classification accuracy as
// well as high performance.

class HandwritingRecSysAlt : public IHandwritingRecSys
{
 public:
  void  train    ( const std::vector<LabeledImage>& v );
  Label classify ( const Image& image );
};

#endif // HRS_ALT_H

