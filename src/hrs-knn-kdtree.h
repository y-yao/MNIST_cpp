//========================================================================
// hrs-knn-kdtree.h
//========================================================================
// Declarations for handwriting recognition systems

#ifndef HRS_KNN_KDTREE_H
#define HRS_KNN_KDTREE_H

#include "types.h"
#include "hrs.h"
#include "kdtree.h"
#include <vector>

//========================================================================
// HandwritingRecSysKnnKdTree
//========================================================================
// This handwriting recognition system uses a k-d tree to accelerate the
// k-nearest neighbors algorithm.
//
// The k-nearest neighbors algorithm searches across the training images
// for the nearest neighbor according to the Euclidean distance and
// classifies the incoming image with the closest neighbor's label.
//
// Because we are using a k-d tree, the closest neighbor may no longer be
// the _exact_ nearest neighbor. This is an example of an "approximate
// k-nearest neighbors" algorithm.

class HandwritingRecSysKnnKdTree : public IHandwritingRecSys
{
 public:
  void  train    ( const std::vector<LabeledImage>& v );
  Label classify ( const Image& image );
 private:

  // Brute-force knn

  Label knn( const Image& image, std::vector<LabeledImage>& v_neighbors );

  // Training set

  std::vector<LabeledImage> m_training_set;

  // K-d tree

  KdTree< image_size,
          LabeledImage,
          std::vector<LabeledImage>::iterator,
          unsigned int > m_kdtree;

  unsigned calculate_sqr_dist( const Image& img0, const Image& img1 ) const;
};

#endif // HRS_KNN_KDTREE_H

