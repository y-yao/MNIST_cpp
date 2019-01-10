//========================================================================
// kdtree-test.cc
//========================================================================
// This file contains the tests for the KdTree

#include <iostream>
#include "kdtree.h"
#include "kdtree-test-misc.h"
#include "utst.h"

int main() {

  UTST_BEGIN();

  //========================================================================
  // Basic Tests
  //========================================================================
  // These are simple test cases to check that the most basic functionality
  // works correctly.
  //
  // Do not add any additional tests here.

  //------------------------------------------------------------------------
  // T1. basic_leaf
  //------------------------------------------------------------------------

  UTST_TEST_LABEL( "T1. basic_leaf" );

  {
    srand(0);

    // Prepare five points

    std::vector<Point> v;
    v.push_back( Point( 7, 2 ) );
    v.push_back( Point( 2, 6 ) );
    v.push_back( Point( 8, 9 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );

    // Build the k-d tree around the points, using leaf size 5. Because
    // there are five points, there will be no cuts.

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 5 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 0, 0 );
    auto expected_neighbor = Point( 3, 4 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }

  //------------------------------------------------------------------------
  // T2. basic_unique_one_cut
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_unique_one_cut
  UTST_TEST_LABEL( "T2. basic_unique_one_cut" );

  {
    srand(0);

    // Prepare five points

    std::vector<Point> v;
    v.push_back( Point( 7, 2 ) );
    v.push_back( Point( 2, 6 ) );
    v.push_back( Point( 8, 9 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );

    // Build the k-d tree around the points, using leaf size 4. Because
    // there are five points, we _must_ make one cut!

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 4 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 0, 0 );
    auto expected_neighbor = Point( 3, 4 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_unique_one_cut

  //------------------------------------------------------------------------
  // T3. basic_nonunique_one_cut
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nonunique_one_cut

  // Test for a corner case during partitioning
  //
  // If _every_ element is less than or equal to the threshold, then all of
  // the elements will go to the left child.
  //
  // - In the best case, all of the elements will go into the left half
  //   and this node is a degenerate node that has no partitioning value.
  //
  // - In the worst case, we have infinite recursion if partitioning in
  //   any possible dimension always hits this corner case.
  //
  // Here is an example of the worst case in two dimensions:
  //
  //   4 |   x   x
  //   3 |
  //   2 |       x
  //   1 |
  //     +---------
  //       1 2 3 4
  //
  // The partitioning scheme will always push all three points into the
  // left partition, no matter which dimension x or y is chosen.
  //
  // To avoid this, we must detect this case (i.e., if the right partition
  // is completely empty then we have hit this corner case) and try to
  // partition with a different threshold (in this case, please just pick
  // the element at idx 0 as the threshold, which is definitely a different
  // value).

  UTST_TEST_LABEL( "T3. basic_nonunique_one_cut" );

  {
    srand(0);

    // Prepare points that trigger the corner case

    std::vector<Point> v;
    v.push_back( Point( 2, 4 ) );
    v.push_back( Point( 4, 2 ) );
    v.push_back( Point( 4, 4 ) );

    // Build the k-d tree around the points, using leaf size 2. Because
    // there are three points, we _must_ make one cut!

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 2 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 0, 4 );
    auto expected_neighbor = Point( 2, 4 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nonunique_one_cut

  //------------------------------------------------------------------------
  // T4. basic_nested_cuts_leafsize2
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_leafsize2
  UTST_TEST_LABEL( "T4. basic_nested_cuts_leafsize2" );

  {
    srand(0);

    // Prepare five points

    std::vector<Point> v;
    v.push_back( Point( 7, 2 ) );
    v.push_back( Point( 2, 6 ) );
    v.push_back( Point( 8, 9 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );

    // Build the k-d tree around the points, using leaf size 2. Because
    // there are five points, we _must_ make multiple cuts!

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 2 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 0, 0 );
    auto expected_neighbor = Point( 3, 4 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_leafsize2

  //------------------------------------------------------------------------
  // T5. basic_nested_cuts_leafsize1
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_leafsize1
  UTST_TEST_LABEL( "T5. basic_nested_cuts_leafsize1" );

  {
    srand(0);

    // Prepare five points

    std::vector<Point> v;
    v.push_back( Point( 7, 2 ) );
    v.push_back( Point( 2, 6 ) );
    v.push_back( Point( 8, 9 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );

    // Build the k-d tree around the points, using leaf size 1. Because
    // there are five points, we _must_ make multiple cuts!

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 1 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 0, 2 );
    auto expected_neighbor = Point( 3, 4 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_leafsize1

  //------------------------------------------------------------------------
  // T6. basic_nested_cuts_10points_leafsize4
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize4
  UTST_TEST_LABEL( "T6. basic_nested_cuts_10points_leafsize4" );

  {
    srand(0);

    // Prepare points

    std::vector<Point> v;
    v.push_back( Point( 4, 4 ) );
    v.push_back( Point( 2, 4 ) );
    v.push_back( Point( 4, 2 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );
    v.push_back( Point( 5, 7 ) );
    v.push_back( Point( 9, 7 ) );
    v.push_back( Point( 8, 7 ) );
    v.push_back( Point( 7, 7 ) );
    v.push_back( Point( 7, 8 ) );

    // Build the k-d tree around the points with leaf size 4

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 4 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 5, 0 );
    auto expected_neighbor = Point( 9, 7 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize4

  //------------------------------------------------------------------------
  // T7. basic_nested_cuts_10points_leafsize3
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize3
  UTST_TEST_LABEL( "T7. basic_nested_cuts_10points_leafsize3" );

  {
    srand(0);

    // Prepare points

    std::vector<Point> v;
    v.push_back( Point( 4, 4 ) );
    v.push_back( Point( 2, 4 ) );
    v.push_back( Point( 4, 2 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );
    v.push_back( Point( 5, 7 ) );
    v.push_back( Point( 9, 7 ) );
    v.push_back( Point( 8, 7 ) );
    v.push_back( Point( 7, 7 ) );
    v.push_back( Point( 7, 8 ) );

    // Build the k-d tree around the points with leaf size 3

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 3 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 9, 0 );
    auto expected_neighbor = Point( 9, 7 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize3

  //------------------------------------------------------------------------
  // T8. basic_nested_cuts_10points_leafsize2
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize2
  UTST_TEST_LABEL( "T8. basic_nested_cuts_10points_leafsize2" );

  {
    srand(0);

    // Prepare points

    std::vector<Point> v;
    v.push_back( Point( 4, 4 ) );
    v.push_back( Point( 2, 4 ) );
    v.push_back( Point( 4, 2 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );
    v.push_back( Point( 5, 7 ) );
    v.push_back( Point( 9, 7 ) );
    v.push_back( Point( 8, 7 ) );
    v.push_back( Point( 7, 7 ) );
    v.push_back( Point( 7, 8 ) );

    // Build the k-d tree around the points with leaf size 2

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 2 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 8, 0 );
    auto expected_neighbor = Point( 8, 7 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize2

  //------------------------------------------------------------------------
  // T9. basic_nested_cuts_10points_leafsize1
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize1
  UTST_TEST_LABEL( "T9. basic_nested_cuts_10points_leafsize1" );

  {
    srand(0);

    // Prepare points

    std::vector<Point> v;
    v.push_back( Point( 4, 4 ) );
    v.push_back( Point( 2, 4 ) );
    v.push_back( Point( 4, 2 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );
    v.push_back( Point( 5, 7 ) );
    v.push_back( Point( 9, 7 ) );
    v.push_back( Point( 8, 7 ) );
    v.push_back( Point( 7, 7 ) );
    v.push_back( Point( 7, 8 ) );

    // Build the k-d tree around the points with leaf size 1

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 1 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 6, 0 );
    auto expected_neighbor = Point( 7, 7 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_10points_leafsize1

  //------------------------------------------------------------------------
  // T10. basic_nested_cuts_more_points
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_more_points
  UTST_TEST_LABEL( "T10. basic_nested_cuts_more_points" );

  {
    srand(0);

    // Prepare points

    std::vector<Point> v;
    v.push_back( Point( 4, 4 ) );
    v.push_back( Point( 2, 4 ) );
    v.push_back( Point( 4, 2 ) );
    v.push_back( Point( 1, 1 ) );
    v.push_back( Point( 3, 4 ) );
    v.push_back( Point( 5, 7 ) );
    v.push_back( Point( 9, 7 ) );
    v.push_back( Point( 8, 7 ) );
    v.push_back( Point( 7, 7 ) );
    v.push_back( Point( 7, 8 ) );
    v.push_back( Point( 4, 5 ) );
    v.push_back( Point( 2, 5 ) );
    v.push_back( Point( 4, 3 ) );
    v.push_back( Point( 1, 2 ) );
    v.push_back( Point( 3, 5 ) );
    v.push_back( Point( 5, 8 ) );
    v.push_back( Point( 9, 8 ) );
    v.push_back( Point( 8, 8 ) );
    v.push_back( Point( 7, 3 ) );
    v.push_back( Point( 7, 9 ) );

    // Build the k-d tree around the points with leaf size 4

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 4 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 0, 8 );
    auto expected_neighbor = Point( 9, 8 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_more_points

  //------------------------------------------------------------------------
  // T11. basic_nested_cuts_many_more_points
  //------------------------------------------------------------------------

  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  // Uncomment this test when you are passing all previous tests
  //
  // Note that you should uncomment "d.draw()" temporarily to check on your
  // cuts! Comment it again to avoid cluttering your screen.
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_many_more_points
  UTST_TEST_LABEL( "T11. basic_nested_cuts_many_more_points" );

  {
    srand(0);

    // Prepare points to fill all 100 spaces

    std::vector<Point> v;
    for ( int i = 0; i <= 9; ++i )
      for ( int j = 0; j <= 9; ++j )
        v.push_back( Point( i, j ) );

    // Build the k-d tree around the points with leaf size 4

    KdTree<2, Point, std::vector<Point>::iterator, int> mytree;

    mytree.set_leaf_size( 4 );
    mytree.build( v.begin(), v.end() );

    // Add the points and k-d tree root to the drawing

    Drawing d;
    for ( Point& p : v )
      d.add( p );

    d.register_root( mytree.get_root() );

    // Uncomment this line to draw the plot

    //d.draw();

    // Verify the neighbor of a given search point

    auto search_point      = Point( 6, 9 );
    auto expected_neighbor = Point( 9, 9 );

    std::vector<Point> v_neighbors;

    mytree.search( search_point, v_neighbors );

    bool found = false;
    for ( auto& item : v_neighbors )
      found = found || (item == expected_neighbor);

    if ( !found ) {
      std::printf(  "\tLine %d: [ \033[31m FAILED \033[0m ] "
                    "Searching neighbors of point (%d, %d)... "
                    "Expected neighbor (%d, %d) was not found!",
                    __LINE__,
                    search_point[0],
                    search_point[1],
                    expected_neighbor[0],
                    expected_neighbor[1] );
      return 1;
    } else {
      std::printf( "\tLine %d: [ \033[32m passed \033[0m ]\n", __LINE__);
    }
  }
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''''
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // basic_nested_cuts_many_more_points
  UTST_END();

  return 0;
}
