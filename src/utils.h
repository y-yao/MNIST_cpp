//========================================================================
// utils.h
//========================================================================
// Useful functions

#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "hrs.h"
#include <string>
#include <vector>

//------------------------------------------------------------------------
// read_labeled_images (reading from MNIST binary file)
//------------------------------------------------------------------------
// Reads a file and fills a container with Images from that file
//
// - fname_images   : file with images to open (binary file is expected)
// - fname_labels   : file with labels to open (binary file is expected)
// - labeled_images : empty container to fill with labeled images
// - size           : number of images in this file

void read_labeled_images( const std::string&               fname_images,
                          const std::string&               fname_labels,
                                std::vector<LabeledImage>* labeled_images,
                                unsigned int               size );

//------------------------------------------------------------------------
// train_and_classify
//------------------------------------------------------------------------
// Takes a handwriting recognition system, trains it with all of the
// labeled images in the training set, classifies all of the images in the
// inference set, and then checks how many classifications are correct,
// returning the accuracy as a double.
//
// - hrs         : a handwriting recognition system to train and run
// - v_training  : labeled images for training
// - v_inference : labeled images to classify and check accuracy with

double train_and_classify( IHandwritingRecSys& hrs,
                           std::vector<LabeledImage>& v_training,
                           std::vector<LabeledImage>& v_inference );

#endif // UTILS_H

