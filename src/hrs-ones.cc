//========================================================================
// hrs-ones.cc
//========================================================================
// Handwriting recognition system member function definitions

#include "hrs-ones.h"
#include <array>
#include <cassert>
#include <cmath>
//========================================================================
// HandwritingRecSysClassifyOnes
//========================================================================
//------------------------------------------------------------------------
// train
//------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void HandwritingRecSysClassifyOnes::train( const std::vector<LabeledImage>& v )
{}
#pragma GCC diagnostic pop

//------------------------------------------------------------------------
// classify
//------------------------------------------------------------------------

Label HandwritingRecSysClassifyOnes::classify( const Image& image )
{
  return detect_ones( image );
}

//------------------------------------------------------------------------
// Detect ones
//------------------------------------------------------------------------

Label HandwritingRecSysClassifyOnes::detect_ones( const Image& image )
{
  //'''' ASSIGNMENT TASK '''''''''''''''''''''''''''''''''''''''''''''''''
  // Implement this function
  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  std::vector<size_t> x_coords, y_coords;  //create two vectors for the x and y coordinates
                                           // of pixels with non-zero values.
  for ( size_t x = 0; x < 28; x++ ) {      // Loop through all x's and y's to find non-zero pixels
    for ( size_t y = 0; y < 28; y++ ) {
      if ( image.at(x, y) > 0 ) {          // If non-zero, store coordinates into vectors
        x_coords.push_back( x );
        y_coords.push_back( y );
      }
    }
  }
  
  if ( linear_regression( x_coords, y_coords ) > 5 )  // If the mean squared error is larger than the threshold
    return Label( '0' );                              // 0 is returned
  else 
    return Label( '1' );                              // otherwise the image is close to a straight line
}                                                     // and 1 is detected

double HandwritingRecSysClassifyOnes::linear_regression( // This function fit the non-zero pixels with a 
 const std::vector<size_t>& x_coords,               // straight line, and output the mean squared error of the fit.
 const std::vector<size_t> y_coords ) const         // The input are vectors of x and y coordinates.
{
  size_t length = x_coords.size();
  double x_bar = 0.;                      // Mean of x coordinates
  double y_bar = 0.;                      // mean of y coordinates
  double xy_bar = 0.;                     // mean of product of x and y
  double yy_bar = 0.;                     // mean of y^2
  for ( size_t i = 0; i < length; i++ ) { // Calculate sum with a loop.
    double x_coord = x_coords[i];
    double y_coord = y_coords[i];
    x_bar += x_coord;
    y_bar += y_coord;
    xy_bar += x_coord * y_coord;
    yy_bar += y_coord * y_coord;
  }
  x_bar /= length;    //Calculate averages by dividing the length
  y_bar /= length;
  xy_bar /= length;
  yy_bar /= length;
  // m and b are the parameters of straight line: y = m*x + b.
  // They are calculated according to linear regression formula.
  double m = ( xy_bar - x_bar * y_bar ) / ( yy_bar - y_bar * y_bar ); 
  double b = x_bar - m * y_bar;

  // calculate mean squared error:
  double res_sqr = 0.;
  for ( size_t i = 0; i < length; i++ ) { // sum the residuals with a loop
    double x_coord = x_coords[i];
    double y_coord = y_coords[i];
    // resisual (error) is define as the shortest distance from the point to
    // the fitted line. Below is the formula
    res_sqr += std::pow(( m * y_coord - x_coord + b ), 2) / ( m * m + 1 );
  }

  return res_sqr / length;  // calculate the mean by dividing the length
}

