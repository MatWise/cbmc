/*******************************************************************\

Module: Structured Text Language Conversion

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Conversion

#include "convert_real_literal.h"
#include <util/ieee_float.h>
#include <util/std_expr.h>

constant_exprt convert_real_literal(const std::string &src)
{
  floatbv_typet type = ieee_float_spect::single_precision().to_type();
  type.set(ID_structured_text_type, ID_structured_text_real);

  return constant_exprt(src, type);
}
