//========================================================================
// kdtree.h
//========================================================================
// Declarations for k-d tree

#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

//========================================================================
// KdTree
//========================================================================
// Class for a templated k-d tree that is polymorphic across the following
// parameters:
//
// - K is the number of dimensions for this K-dimensional tree. This means
//   that each item in the k-d tree exists in K-dimensional space.
//
// - T is the type of the data contained in the tree.
//
// - Itr is the type of an iterator over T for some generic container.
//
// - V is the type of T's value along a dimension. For example in one
//   dimensional space if points exist at whole numbers (e.g., an
//   x-coordinate of 2), then V is an int type. If instead, x-coordinates
//   can be doubles (e.g., an x-coordinate of 2.35), then V is a double
//   type. For an MNIST image, V is an unsigned integer type because the
//   value at each dimension (i.e., each grayscale pixel) is an unsigned
//   integer between 0 and 255. Note that V is also the type used to store
//   the threshold value.

template <unsigned int K, typename T, typename Itr, typename V>
class KdTree
{
 public:

  // Node struct to create a k-d tree

  struct Node
  {
    Node*        m_left_p;     // Left child node
    Node*        m_right_p;    // Right child node

    unsigned int m_dim;        // Chosen dimension for partition
    V            m_threshold;  // Threshold value to choose left vs. right

    bool         m_is_leaf;    // Is this a leaf node?
    const Itr    m_data_start; // Start iterator for data
    const Itr    m_data_end;   // End iterator for data

    Node();
    Node( Itr start, Itr end );
  };

  // Constructors and destructors

  KdTree();
  ~KdTree();

  //----------------------------------------------------------------------
  // search
  //----------------------------------------------------------------------
  // Walk the tree until we find the leaf node in which the given item
  // belongs. Push back all of the leaf node's data into the provided
  // vector.

  void search( T& item, std::vector<T>& v_data );

  //----------------------------------------------------------------------
  // build
  //----------------------------------------------------------------------
  // Build a k-d tree to organize the provided items.
  //
  // The k-d tree is a binary tree, where data only lives at leaf nodes.
  //
  // Each node has a chosen dimension in the range [0, k). Each node
  // chooses a threshold value and then partitions the data between left
  // and right branches along that dimension using the threshold value.
  //
  // - All data in the left branch has values less than or equal to the
  //   threshold value in that dimension.
  //
  // - All data in the right branch has values greater than the threshold
  //   value in that dimension.
  //
  // The threshold value is picked by sorting the items and picking the
  // median in order to partition the items roughly in half. Note that if
  // there are many non-unique items (e.g., all zeros), then the
  // partitions are likely to become imbalanced, since all items equal to
  // the median in the chosen dimension must go into the same partition.
  //
  // The leaf node size parameter determines the base case of the
  // recursive build and is a key parameter that can be chosen to tune
  // performance versus accuracy. While building the tree, the data is
  // partitioned roughly in half to the left and right branches. When the
  // size of the data in a node is less than the leaf node size, the
  // partitioning stops and the node is labeled as a leaf node. Only leaf
  // nodes contain data.

  void build( Itr start, Itr end );

  // Set leaf size

  void set_leaf_size( unsigned int leaf_size );

  // Get root node

  Node*& get_root();

 private:

  // Member fields

  unsigned int       m_leaf_size; // leaf size
  std::vector<Node*> m_nodelist;  // bookkeeping for destructor
  Node*              m_root_p;

  // Private member functions

  // Recursive helper kernel for build

  Node* build_kernel( Itr start, Itr end );

  // Choose one of K dimensions for partition

  unsigned int choose_dimension( Itr start, Itr end );

  // Partition along the chosen dimension

  Itr partition( Itr start, Itr end, unsigned int dim, Node* node_p );
};

// Definitions for inline functions

#include "kdtree.inl"

#endif // KDTREE_H

