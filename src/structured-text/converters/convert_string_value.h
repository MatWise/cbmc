/*******************************************************************\

Module: Structured Text Language Conversion

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Conversion

#ifndef CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_STRING_VALUE_H
#define CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_STRING_VALUE_H

#include <util/expr.h>
#include <util/std_code.h>
#include <util/string_constant.h>

/// Converts a string into a Structured Text identifier.
/// \param src: String returned by the parser.
/// \return Constant string expression representing the identifier.
string_constantt convert_identifier(const std::string &src);

/// Converts a string into a Structured Text title.
/// \param src: String returned by the parser.
/// \return Constant string expression representing the title.
string_constantt convert_title(const std::string &src);

/// Converts a string into a Structured Text label.
/// \param src: String returned by the parser.
/// \return Code label expression representing the label.
code_labelt convert_label(const std::string &src);

#endif // CPROVER_STRUCTURED_TEXT_CONVERTERS_CONVERT_STRING_VALUE_H
