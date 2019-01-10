//========================================================================
// types.h
//========================================================================
// Declarations for misc types

#ifndef TYPES_H
#define TYPES_H

#include "constants.h"
#include <iostream>
#include <array>

//------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------
// An image has an array of data

class Image
{
 public:
  typedef std::array<unsigned int, image_size>::const_iterator ConstItr;
  typedef std::array<unsigned int, image_size>::iterator       Itr;

  Image();
  Image( std::array<unsigned int, image_size>& data );

  void   print() const;
  size_t size()  const;

  ConstItr begin() const;
  ConstItr end()   const;

  unsigned int at        ( size_t x, size_t y ) const;
  unsigned int operator[]( size_t idx         ) const;

 private:
  std::array<unsigned int, image_size> m_data;
};

//------------------------------------------------------------------------
// Label
//------------------------------------------------------------------------
// An label just has a simple field

class Label
{
 public:
  static const char INVALID = '?';

  Label();
  Label( char label );

  void print() const;

  char get_label() const;
  void set_label( char label );

 private:
  char m_label;
};

//------------------------------------------------------------------------
// LabeledImage
//------------------------------------------------------------------------
// A labeled image has an image and a label

class LabeledImage
{
 public:

  LabeledImage();
  LabeledImage( const Image& image, const Label& label );

  void print() const;

  Image::ConstItr begin() const;
  Image::ConstItr end()   const;

  char         get_label() const;
  const Image& get_image() const;

  unsigned int at        ( size_t x, size_t y ) const;
  unsigned int operator[]( size_t idx         ) const;

 private:
  Image m_image;
  Label m_label;
};

// This file contains all inlined member functions

#include "types.inl"

#endif // TYPES_H

