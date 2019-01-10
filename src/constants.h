//========================================================================
// constants.h
//========================================================================

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdlib>

const size_t n_rows     = 28;            // MNIST has 28 rows
const size_t n_cols     = 28;            // MNIST has 28 columns
const size_t image_size = 28 * 28;       // MNIST has 28 x 28 pixels

const double accuracy_precision = 0.015; // Precision for checking
                                         // expected accuracy for tests

#endif // CONSTANTS_H

