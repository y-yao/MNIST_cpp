//========================================================================
// kdtree.inl
//========================================================================
// Definitions for kdtree (inlined)

#include <vector>
#include <algorithm>

//========================================================================
// KdTree::Node
//========================================================================
//========================================================================
// Constructors
//========================================================================

template <unsigned int K, typename T, typename Itr, typename V>
KdTree<K, T, Itr, V>::Node::Node()
  : m_left_p     ( nullptr ),
    m_right_p    ( nullptr ),
    m_is_leaf    ( false   ),
    m_data_start ( Itr()   ),
    m_data_end   ( Itr()   )
{}

template <unsigned int K, typename T, typename Itr, typename V>
KdTree<K, T, Itr, V>::Node::Node( Itr start, Itr end )
  : m_left_p     ( nullptr ),
    m_right_p    ( nullptr ),
    m_is_leaf    ( false   ),
    m_data_start ( start   ),
    m_data_end   ( end     )
{}

//========================================================================
// KdTree
//========================================================================
//------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------

template <unsigned int K, typename T, typename Itr, typename V>
KdTree<K, T, Itr, V>::KdTree()
  : m_leaf_size ( 1000    ), // Default leaf node size
    m_root_p    ( nullptr )
{}

//------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------

template <unsigned int K, typename T, typename Itr, typename V>
KdTree<K, T, Itr, V>::~KdTree()
{
  for (auto np: m_nodelist) {   // delete all nodes in nodelist
    delete np;
  }
}

//------------------------------------------------------------------------
// search
//------------------------------------------------------------------------

template <unsigned int K, typename T, typename Itr, typename V>
void KdTree<K, T, Itr, V>::search( T& item, std::vector<T>& v_data )
{
  Node* np = m_root_p;                          // initialize the pointer to the root
  while ( !(np->m_is_leaf) ) {                  // Before it gets to the leaf
    if ( item[np->m_dim] <= np->m_threshold ) { // if the item's value in the current node's dim is 
                                                // no greater than the threshold,
      np = np->m_left_p;                        // move the current pointer to the left node
    } else {
      np = np->m_right_p;                       // if not, move to the right node
    }
  }
  for ( Itr itr = np->m_data_start; itr < np->m_data_end; ++itr ) { // push all image in the resulting
    v_data.push_back(*itr);                                         // leaf to the v_data vector
  }
}

//------------------------------------------------------------------------
// build
//------------------------------------------------------------------------

template <unsigned int K, typename T, typename Itr, typename V>
inline void KdTree<K, T, Itr, V>::build( Itr start, Itr end )
{
  m_root_p = build_kernel( start, end );
}

//------------------------------------------------------------------------
// build_kernel
//------------------------------------------------------------------------

template <unsigned int K, typename T, typename Itr, typename V>
typename KdTree<K, T, Itr, V>::Node*
  KdTree<K, T, Itr, V>::build_kernel( Itr start, Itr end )
{
  unsigned int size = std::distance( start, end );

  //----------------------------------------------------------------------
  // Base case: No elements
  //----------------------------------------------------------------------

  if ( size == 0 ) return nullptr;

  //----------------------------------------------------------------------
  // Build Step 1: Base case: Leaf node
  //----------------------------------------------------------------------
  if ( size <= m_leaf_size ) {            // If number of points is less than the max number for a leaf node
    Node* node_p = new Node(start, end);  // a new node is created on heap
    node_p->m_is_leaf = true;             // and it is set to be leaf
    m_nodelist.push_back(node_p);         // push it to nodelist for easy deletion
    return node_p;
  }

  //----------------------------------------------------------------------
  // Build Step 2: Create a new node
  //----------------------------------------------------------------------
  Node* node_p = new Node(start, end);    // make a new intermediate node on heap
  m_nodelist.push_back(node_p);

  //----------------------------------------------------------------------
  // Build Step 3: Choose one of K dimensions for partition
  //----------------------------------------------------------------------
  // Choose a good dimension to partition along. More detail is provided
  // in the comments inside the function.
  //----------------------------------------------------------------------
  unsigned dim = choose_dimension(start, end);  // find a n appropriate dimension
  node_p->m_dim = dim;                          // assign selected dim to the node

  //----------------------------------------------------------------------
  // Build Step 4: Partition along the chosen dimension
  //----------------------------------------------------------------------
  Itr med_ptr = partition(start, end, dim, node_p);  // find the partition point
                                                     // left most image in the right partition
  //----------------------------------------------------------------------
  // Build Step 5: Recursive steps
  //----------------------------------------------------------------------

  node_p->m_left_p = build_kernel(start, med_ptr);    // recursively build the left subtree
  node_p->m_right_p = build_kernel(med_ptr, end);      // recursively build the right subtree

  return node_p;
}

//------------------------------------------------------------------------
// Choose one of K dimensions for partition
//------------------------------------------------------------------------
template <unsigned int K, typename T, typename Itr, typename V>
unsigned int KdTree<K, T, Itr, V>::choose_dimension( Itr start, Itr end )
{
  // Find a nice dimension to split along
  //
  // In particular, we don't want to choose a dimension where all elements
  // have the exact same value, because then there is no meaningful
  // partitioning.
  //
  // For example, in two dimensions, if all elements have the same x
  // position, then it is not meaningful to partition along the x axis:
  //
  //     4 |   x
  //     3 |   x
  //     2 |   x
  //     1 |   x
  //       +---------
  //         1 2 3 4
  //
  // In this situation, we should re-roll and choose another dimension to
  // partition along (e.g., the y axis).
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  unsigned int dim = rand() % K;  // randomly select a dimension from 0 to K

  bool is_good_dim = false;       // state variable indicating whether it is a good dimension
  while ( !is_good_dim ) {
    for ( Itr itr = start; itr < end - 1 ; ++itr ) { // go through values in the selected dimension
      if ((*itr)[dim] != (*(itr + 1))[dim]) {        // if this dimemsion has any two different consecutive values
        is_good_dim = true;                          // then this dimemsion is good
        break;                        
      }
    }
    if ( !is_good_dim ){           // If all values are the same (not good dim)
      dim = rand() % K;            // another dimension is randomly selected
    }
  }

  return dim;
}

//------------------------------------------------------------------------
// Partition along the chosen dimension
//------------------------------------------------------------------------
// Partition the elements along the chosen dimension into a left partition
// and a right partition.

template <unsigned int K, typename T, typename Itr, typename V>
Itr KdTree<K, T, Itr, V>::partition( Itr          start,
                                     Itr          end,
                                     unsigned int dim,
                                     Node*        node_p )
{
  // using std::sort to sort the image vector according to values in selected dimension
  std::sort(start, end, [&dim](const T& left, const T& right){ return left[dim] < right[dim]; });
  size_t size = std::distance(start, end);  // Get the size of image vector

  V threshold;
  threshold = (*(start + (size / 2)))[dim];  // get the threshold (mediam) by getting the value of middle image
  //Itr std::partition(start, end, [threshold](const auto& elem){ return elem[dim]
 
  auto partition_pred = [&dim, &threshold](const T& item){ return item[dim] <= threshold; };

  Itr partition_itr = std::partition(start, end, partition_pred);
  if ( std::distance(partition_itr, end) == 0 ) {
    threshold = (*start)[dim];
    partition_itr = std::partition(start, end, partition_pred);
  }
  node_p->m_threshold = threshold;             // the field of node is cupdated accordingly
  return partition_itr;
  //return start + ((size+1)/2);                 // The iterator of middle image is returned
}

//------------------------------------------------------------------------
// set_leaf_size
//------------------------------------------------------------------------
template <unsigned int K, typename T, typename Itr, typename V>
void KdTree<K, T, Itr, V>::set_leaf_size( unsigned int leaf_size )
{
  m_leaf_size = leaf_size;
}

//------------------------------------------------------------------------
// get_root
//------------------------------------------------------------------------
template <unsigned int K, typename T, typename Itr, typename V>
typename KdTree<K, T, Itr, V>::Node*& KdTree<K, T, Itr, V>::get_root()
{
  return m_root_p;
}

