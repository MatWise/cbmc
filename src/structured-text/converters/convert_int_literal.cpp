/*******************************************************************\

Module: Structured Text Language Conversion

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Conversion

#include "convert_int_literal.h"
#include <algorithm>
#include <util/std_types.h>

constant_exprt convert_int_dec_literal(const std::string &src)
{
  // Remove '+' char if present.
  std::string value(src);
  value.erase(std::remove(value.begin(), value.end(), '+'), value.end());

  signedbv_typet type{16};
  type.set(ID_structured_text_type, ID_structured_text_int);

  return constant_exprt(value, type);
}

constant_exprt convert_int_hex_literal(const std::string &src)
{
  // Cut the leading '16#' and convert to int
  std::string cut = src.substr(3, std::string::npos);
  int value = std::stoi(cut, nullptr, 16);

  signedbv_typet type{16};
  type.set(ID_structured_text_type, ID_structured_text_int);

  return constant_exprt(std::to_string(value), type);
}

constant_exprt convert_int_bit_literal(const std::string &src)
{
  // Cut the leading '2#' and convert to int
  std::string cut = src.substr(2, std::string::npos);
  int value = std::stoi(cut, nullptr, 2);

  signedbv_typet type{16};
  type.set(ID_structured_text_type, ID_structured_text_int);

  return constant_exprt(std::to_string(value), type);
}
