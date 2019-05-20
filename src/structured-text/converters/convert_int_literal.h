/*******************************************************************\

Module: Structured Text Language Conversion

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Conversion

#ifndef CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_INT_LITERAL_H
#define CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_INT_LITERAL_H

#include <string>

#include <util/expr.h>
#include <util/std_expr.h>

/// Converts a string into the corresponding 'Int' expression.
/// \param src: String returned by the parser (base 10).
/// \return Constant expression representing the integer value.
constant_exprt convert_int_dec_literal(const std::string &src);

/// Converts a string into the corresponding 'Int' expression.
/// \param src: String returned by the parser (base 16).
/// \return Constant expression representing the integer value.
constant_exprt convert_int_hex_literal(const std::string &src);

/// Converts a string into the corresponding 'Int' expression.
/// \param src: String returned by the parser (base 2).
/// \return Constant expression representing the integer value.
constant_exprt convert_int_bit_literal(const std::string &src);

#endif // CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_INT_LITERAL_H
