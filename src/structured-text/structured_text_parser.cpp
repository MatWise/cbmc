/*******************************************************************\

Module: Structured Text Language Parser

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Parser

#include "structured_text_parser.h"
#include "structured_text_parse_tree.h"
#include <cmath>
#include <iostream>
#include <util/expr.h>

structured_text_parsert structured_text_parser;

extern char *yystructured_texttext;

int yystructured_texterror(const std::string &error)
{
  structured_text_parser.parse_error(error, yystructured_texttext);
  return 0;
}

void structured_text_parsert::add_function_block(exprt &block)
{
  INVARIANT(
    block.id() == ID_structured_text_function_block,
    "Root expression ID should be ID_structured_text_function_block");

  // Generate new function block.
  structured_text_parse_treet::function_blockt fb{find_name(block),
                                                  find_version(block)};

  // Fill the block with networks and variables.
  exprt var_list = find_variable_list(block);
  find_variables(fb, var_list);
  exprt network_list = find_network_list(block);
  find_networks(fb, network_list);

  parse_tree.add_function_block(fb);
}

void structured_text_parsert::add_function(exprt &function)
{
  INVARIANT(
    function.id() == ID_structured_text_function,
    "Expression ID should be structured_text_function");

  // Generate new function.
  structured_text_parse_treet::functiont fn{find_name(function),
                                            find_version(function)};

  // Fill the function with networks and variables.
  exprt var_decls = find_variable_list(function);
  find_variables(fn, var_decls);
  exprt network_list = find_network_list(function);
  find_networks(fn, network_list);

  parse_tree.add_function(fn);
}

void structured_text_parsert::clear()
{
  parsert::clear();
  parse_tree.clear();
}

void structured_text_parsert::print_tree(std::ostream &out) const
{
  parse_tree.output(out);
}

irep_idt structured_text_parsert::find_name(const exprt &root)
{
  for(exprt op : root.operands())
  {
    if(op.get(ID_structured_text_type) == ID_structured_text_identifier)
      return op.get(ID_value);
  }

  INVARIANT(false, "Root expression should have a name");
  return irep_idt();
}

float structured_text_parsert::find_version(const exprt &root)
{
  for(exprt op : root.operands())
  {
    if(op.type().id() == ID_structured_text_version)
      return std::stof(op.get(ID_value).c_str());
  }

  INVARIANT(false, "Root expression should have a version");
  return -1;
}

exprt structured_text_parsert::find_variable_list(const exprt &root)
{
  for(exprt op : root.operands())
  {
    if(op.id() == ID_structured_text_var_decls)
      return op;
  }
  INVARIANT(false, "Root expression should have a variable list");
  return exprt(ID_nil);
}

exprt structured_text_parsert::find_network_list(const exprt &root)
{
  for(exprt op : root.operands())
  {
    if(op.id() == ID_structured_text_networks)
      return op;
  }

  INVARIANT(false, "Root expression should have a network list");
  return exprt(ID_nil);
}

void structured_text_parsert::find_variables(
  structured_text_parse_treet::functiont &function,
  const exprt &var_decls)
{
  for(exprt decls : var_decls.operands())
  {
    if(decls.id() == ID_structured_text_var_input)
      fill_input_vars(function, decls);
    else if(decls.id() == ID_structured_text_var_inout)
      fill_output_vars(function, decls);
    else if(decls.id() == ID_structured_text_var_output)
      fill_output_vars(function, decls);
  }
}

void structured_text_parsert::fill_input_vars(
  structured_text_parse_treet::functiont &function,
  const exprt &input_vars)
{
  for(exprt entry : input_vars.operands())
  {
    irep_idt identifier;
    typet type;
    for(exprt part : entry.operands())
    {
      if(part.get(ID_structured_text_type) == ID_structured_text_identifier)
        identifier = part.get(ID_value);
      else
        type = typet(part.id());
    }

    structured_text_parse_treet::var_declarationt declaration(identifier, type);
    function.add_var_input_entry(declaration);
  }
}

void structured_text_parsert::fill_inout_vars(
  structured_text_parse_treet::functiont &function,
  const exprt &inout_vars)
{
  for(exprt entry : inout_vars.operands())
  {
    irep_idt identifier;
    typet type;
    for(exprt part : entry.operands())
    {
      if(part.get(ID_structured_text_type) == ID_structured_text_identifier)
        identifier = part.get(ID_value);
      else
        type = typet(part.id());
    }

    structured_text_parse_treet::var_declarationt declaration(identifier, type);
    function.add_var_inout_entry(declaration);
  }
}

void structured_text_parsert::fill_output_vars(
  structured_text_parse_treet::functiont &function,
  const exprt &output_vars)
{
  for(exprt entry : output_vars.operands())
  {
    irep_idt identifier;
    typet type;
    for(exprt part : entry.operands())
    {
      if(part.get(ID_structured_text_type) == ID_structured_text_identifier)
        identifier = part.get(ID_value);
      else
        type = typet(part.id());
    }

    structured_text_parse_treet::var_declarationt declaration(identifier, type);
    function.add_var_output_entry(declaration);
  }
}

void structured_text_parsert::find_networks(
  structured_text_parse_treet::functiont &function,
  const exprt &network_list)
{
  for(exprt expr_network : network_list.operands())
  {
    std::string title(find_network_title(expr_network));
    structured_text_parse_treet::networkt network(title);
    exprt instructions = find_network_instructions(expr_network);
    find_instructions(network, instructions);
    function.add_network(network);
  }
}

std::string structured_text_parsert::find_network_title(const exprt &network)
{
  for(exprt network_element : network.operands())
  {
    if(network_element.get(ID_structured_text_type) == ID_structured_text_title)
      return network_element.get(ID_value).c_str();
  }
  INVARIANT(false, "Network expression should have a title");
  return "";
}

exprt structured_text_parsert::find_network_instructions(const exprt &network)
{
  for(exprt network_element : network.operands())
  {
    if(network_element.id() == ID_structured_text_instructions)
      return network_element;
  }
  INVARIANT(false, "Network expression should have an instruction list");
  return exprt{ID_nil};
}

void structured_text_parsert::find_instructions(
  structured_text_parse_treet::networkt &network,
  const exprt &instructions)
{
  for(exprt expr_instruction : instructions.operands())
  {
    structured_text_parse_treet::instructiont instruction;
    codet code_token(expr_instruction.op0().id());
    for(exprt expr : expr_instruction.operands())
    {
      if(expr.id() != ID_nil && expr.id() != code_token.get_statement())
        code_token.add_to_operands(expr);
    }
    instruction.add_token(code_token);
    network.add_instruction(instruction);
  }
}

void structured_text_parsert::swap_tree(structured_text_parse_treet &other)
{
  parse_tree.swap(other);
}
