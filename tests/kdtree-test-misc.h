#include <iostream>

//========================================================================
// Point
//========================================================================
// A two-dimensional point class

class Point
{
 public:
  int x[2]; // Data along each of the two dimensions

  // Constructors

  Point()                 { x[0] = -1; x[1] = -1; }
  Point( int _x, int _y ) { x[0] = _x; x[1] = _y; }

  // Access data with operator[]

  const int& operator[]( size_t idx ) const { return x[idx]; }
};

//------------------------------------------------------------------------
// Overloaded operators
//------------------------------------------------------------------------

std::ostream& operator<<( std::ostream& os, const Point& p )
{
  os << "(" << p.x[0] << ", " << p.x[1] << ")";
  return os;
}

bool operator==( const Point& lhs, const Point& rhs )
{
  return ( lhs.x[0] == rhs.x[0] ) && ( lhs.x[1] == rhs.x[1] );
}

//========================================================================
// Drawing
//========================================================================
// A drawing class that draws two-dimensional axes in the x-range of [0,
// 9] and the y-range of [0, 9].
//
// It can also register a k-d tree node and walk the tree to find all the
// thresholds, and then draw the thresholds.
//
// This class uses cursor movement ANSI escape codes to:
//
// 1. Reserve "m_lines" lines of output
// 2. Draw the frame      : Backtrack "m_lines" lines, draw the frame
// 3. Draw all points     : Backtrack "m_lines" lines, draw the point
// 4. Draw all thresholds : Backtrack "m_lines" lines, draw the threshold

// Cursor movement magic

const char* cursor_u = "\033[A";
const char* cursor_d = "\033[B";

const char* cursor_r  = "\033[C";
const char* cursor_r2 = "\033[2C";
const char* cursor_r4 = "\033[4C";

const char* cursor_l  = "\033[D";
const char* cursor_l2 = "\033[2D";
const char* cursor_l4 = "\033[4D";

const size_t max_points = 100;

class Drawing
{
 public:
  typedef KdTree<2, Point, std::vector<Point>::iterator, int>::Node* NodeP;

  // Member fields

  std::vector<Point> m_data;
  NodeP              m_root_p;

  // Number of lines to reserve for output. For an x-range of [0, 9] and a
  // y-range of [0, 9], we need two lines per row + two lines for x-axis.

  const int m_lines = 2*10 + 2;

  //------------------------------------------------------------------------
  // Constructor
  //------------------------------------------------------------------------

  Drawing(){}

  //------------------------------------------------------------------------
  // register_root
  //------------------------------------------------------------------------
  // Save the root of a k-d tree. We will use this root to traverse the
  // tree later and print out all of the partitions.

  void register_root( NodeP& foo )
  {
    m_root_p = foo;
  }

  //------------------------------------------------------------------------
  // add
  //------------------------------------------------------------------------
  // Add a point to the drawing

  void add( Point& p )
  {
    for ( auto& value : p.x )
      if ( value < 0 || value > 9 )
        throw std::out_of_range(
            "Points must be in the x-range / y-range of [0, 9])!" );

    m_data.push_back( p );
  }

  //------------------------------------------------------------------------
  // draw_frame
  //------------------------------------------------------------------------
  // Draw the frame with the x-axis and y-axis

  void draw_frame()
  {
    // Backtrack lines

    for ( int i = 0; i < m_lines; ++i )
      std::cout << cursor_u;

    // Print all the rows

    for ( int y = 9; y >= 0; --y ) {
      std::cout << " " << " | " << std::endl;
      std::cout <<  y  << " | " << std::endl;
    }

    // Print x axis

    std::cout << "  +-----------------------------------------" << std::endl;
    std::cout << "    0   1   2   3   4   5   6   7   8   9   " << std::endl;
  }

  //------------------------------------------------------------------------
  // draw_point
  //------------------------------------------------------------------------
  // Draw a single point

  void draw_point( Point& p )
  {
    // Backtrack lines

    for ( int i = 0; i < m_lines; ++i )
      std::cout << cursor_u;

    // Move down to the correct row

    for ( int y = 9; y >= 0; --y ) {
      std::cout << std::endl;
      if ( y == p.x[1] ) {

        // Move right to the correct column

        std::cout << cursor_r4;
        for ( int x = 0; x <= 9; ++x ) {
          if ( x == p.x[0] ) {
            std::cout << "x";
            break;
          }
          else std::cout << cursor_r4;
        }

      }
      std::cout << std::endl;
    }

    // Move down past the x axes

    std::cout << std::endl << std::endl;
  }

  //------------------------------------------------------------------------
  // draw_thresholds
  //------------------------------------------------------------------------
  // Walk the k-d tree to grab all the thresholds and draw all of the
  // thresholds within each partition

  void draw_thresholds()
  {
    draw_thresholds_helper( m_root_p, 0, 9, 0, 9 );
  }

  //------------------------------------------------------------------------
  // draw_thresholds_helper
  //------------------------------------------------------------------------
  // As we walk the k-d tree and enter nested partitions, we narrow the
  // range of [x0, x1] for horizontal thresholds and [y0, y1] for vertical
  // thresholds.
  //
  // This helper is recursively called on both children of the given node.

  void draw_thresholds_helper( NodeP node_p, int x0, int x1, int y0, int y1 )
  {
    if ( node_p == nullptr ) return;
    if ( node_p->m_is_leaf ) return;

    //..........................................................
    // Is this threshold horizontal or vertical?
    //..........................................................

    auto threshold = node_p->m_threshold;

    bool draw_hline = false;
    bool draw_vline = false;

    if ( node_p->m_dim == 0 ) draw_vline = true;
    else                      draw_hline = true;

    //..........................................................
    // Backtrack lines
    //..........................................................

    for ( int i = 0; i < m_lines; ++i )
      std::cout << cursor_u;

    //..........................................................
    // Draw horizontal threshold
    //..........................................................

    if ( draw_hline ) {

      // Move down to the correct row

      for ( int y = 9; y >= 0; --y ) {
        if ( y == threshold ) {

          // Draw the hline in the bounds [x0, x1]

          std::cout << cursor_r4;
          for ( int x = 0; x <= 9; ++x ) {
            if ( x >= x0 && x <= x1 )
              std::cout << "....";
            else
              std::cout << cursor_r4;
          }
        }

        // Skip down two lines to the next y-value

        std::cout << std::endl << std::endl;
      }
    }

    //..........................................................
    // Draw vertical threshold
    //..........................................................

    else if ( draw_vline ) {

      // Draw the vline for all rows in the bounds [y0, y1]

      for ( int y = 9; y >= 0; --y ) {
        if ( y >= y0 && y <= y1 ) {
          std::cout << cursor_r4;

          // Move to the correct column and draw a marker on both rows for
          // this y index.

          for ( int x = 0; x <= 9; ++x ) {
            if ( x == threshold ) {
              std::cout << cursor_r2 << "." << cursor_r;
              std::cout << cursor_l4;
              std::cout << cursor_d;
              std::cout << cursor_r2 << "." << cursor_r;
              std::cout << cursor_l4;
              std::cout << cursor_u;
            }
            else std::cout << cursor_r4;
          }
        }

        // Skip down two lines to the next y-value

        std::cout << std::endl << std::endl;
      }
    }

    // Move down past the x axes

    std::cout << std::endl << std::endl;

    // Recursively draw thresholds of left and right children nodes

    if ( node_p->m_dim == 0 ) {
      draw_thresholds_helper( node_p->m_left_p,  x0, threshold, y0, y1 );
      draw_thresholds_helper( node_p->m_right_p, threshold+1, x1, y0, y1 );
    }
    else {
      draw_thresholds_helper( node_p->m_left_p,  x0, x1, y0, threshold );
      draw_thresholds_helper( node_p->m_right_p, x0, x1, threshold+1, y1 );
    }
  }

  //------------------------------------------------------------------------
  // draw
  //------------------------------------------------------------------------
  // This is the top-level draw function which draws the frame, all
  // points, and all thresholds.

  void draw()
  {
    // Reserve the space for lines of printouts

    for ( int i = 0; i < m_lines; ++i )
      std::cout << std::endl;

    // Draw frame

    draw_frame();

    // Draw all registered points

    for ( auto& item : m_data )
      draw_point( item );

    // Draw all registered thresholds

    draw_thresholds();
  }

};

