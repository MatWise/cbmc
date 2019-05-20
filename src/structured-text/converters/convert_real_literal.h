/*******************************************************************\

Module: Structured Text Language Conversion

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Conversion

#ifndef CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_REAL_LITERAL_H
#define CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_REAL_LITERAL_H

#include <string>

#include <util/expr.h>
#include <util/std_types.h>

/// Converts a string into the corresponding 'Real' expression.
/// \param src: String returned by the parser.
/// \return Constant expression representing the real value.
constant_exprt convert_real_literal(const std::string &src);

#endif // CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_REAL_VALUE_H
