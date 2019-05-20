/*******************************************************************\

Module: Structured Text Language Conversion

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Conversion

#include "convert_bool_literal.h"
#include <algorithm>
#include <util/std_types.h>

constant_exprt convert_bool_literal(const std::string &src)
{
  // Transform to lower case.
  std::string copy;
  std::transform(src.begin(), src.end(), copy.begin(), ::tolower);

  bool_typet type;
  type.set(ID_structured_text_type, ID_structured_text_bool);

  return constant_exprt(copy, type);
}
