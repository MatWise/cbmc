/*******************************************************************\

Module: Conversion from Expression or Type to Structured Text

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

#ifndef CPROVER_STRUCTURED_TEXT_CONVERTERS_EXPR2STRUCTURED_TEXT_H
#define CPROVER_STRUCTURED_TEXT_CONVERTERS_EXPR2STRUCTURED_TEXT_H

#include <string>

/// Converts a given expression to human-readable STL code.
/// \param expr: Expression to be converted.
/// \result String with the STL representation of the given parameters.
std::string expr2stl(const class exprt &expr);

/// Converts a given type to human-readable STL code.
/// \param type: Type to be converted.
/// \result String with the STL representation of the given type.
std::string type2stl(const class typet &type);

/// Converts a given type and identifier to human-readable STL code.
/// \param type: Type to be converted.
/// \param identifier: Identifier to be converted.
/// \result String with the STL representation of the given parameters.
std::string type2stl(const typet &type, const std::string &identifier);

#endif // CPROVER_STRUCTURED_TEXT_CONVERTERS_EXPR2STRUCTURED_TEXT_H
