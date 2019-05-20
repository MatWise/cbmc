/*******************************************************************\

Module: Structured Text Language Type Checking

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// STL Language Type Checking

#include "structured_text_typecheck.h"

#include <util/ieee_float.h>
#include <util/std_code.h>
#include <util/std_expr.h>
#include <util/std_types.h>

bool structured_text_typecheck(
  structured_text_parse_treet &parse_tree,
  symbol_tablet &symbol_table,
  const std::string &module,
  message_handlert &message_handler)
{
  structured_text_typecheckt stl_typecheck(
    parse_tree, symbol_table, module, message_handler);

  return stl_typecheck.typecheck_main();
}

void structured_text_typecheckt::typecheck()
{
  for(const auto &fn : parse_tree.functions)
  {
    typecheck_function(fn);
  }

  for(const auto &fb : parse_tree.function_blocks)
  {
    typecheck_function_block(fb);
  }
}

void structured_text_typecheckt::typecheck_function_block(
  const structured_text_parse_treet::function_blockt &function_block)
{
  symbolt fb_sym;
  fb_sym.module = module;
  code_typet::parameterst params = typecheck_fb_params(function_block);
  typet return_type(ID_void);
  fb_sym.type = code_typet(params, return_type);
  symbol_table.add(fb_sym);
  typecheck_structured_text_networks(function_block.networks);
  // TODO: Expand.
}

void structured_text_typecheckt::typecheck_function(
  const structured_text_parse_treet::functiont &function)
{
  // TODO: Implement.
  (void)function;
}

code_typet::parameterst structured_text_typecheckt::typecheck_fb_params(
  const structured_text_parse_treet::function_blockt &function_block)
{
  code_typet::parameterst params;

  // Input parameters (read-only, are copied* to the instance data block upon
  // calling it)
  // *TODO: Implement call by reference for structured data types like strings.
  for(const auto &declaration : function_block.var_input)
  {
    typet converted_type =
      typecheck_structured_text_type(declaration.variable.type());
    code_typet::parametert param{converted_type};
    params.push_back(param);
  }

  // Inout parameters (read and write)
  for(const auto &declaration : function_block.var_output)
  {
    typet converted_type =
      typecheck_structured_text_type(declaration.variable.type());
    code_typet::parametert param{converted_type};
    params.push_back(param);
  }

  // Output parameters (write-only, are read by the caller after fb execution)
  for(const auto &declaration : function_block.var_output)
  {
    typet converted_type =
      typecheck_structured_text_type(declaration.variable.type());
    code_typet::parametert param{converted_type};
    params.push_back(param);
  }
  return params;
}

typet structured_text_typecheckt::typecheck_structured_text_type(
  const typet &stl_type)
{
  // TODO: Expand.
  return std::move(stl_type);
}

void structured_text_typecheckt::typecheck_structured_text_networks(
  const structured_text_parse_treet::networkst &networks)
{
  // TODO: Implement.
  (void)networks;
}
