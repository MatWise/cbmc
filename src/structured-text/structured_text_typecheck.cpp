/*******************************************************************\

Module: Structured Text Language Type Checking

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// STL Language Type Checking

#include "structured_text_typecheck.h"

#include <util/ieee_float.h>
#include <util/message.h>
#include <util/std_code.h>
#include <util/std_expr.h>
#include <util/std_types.h>

#define STRUCTURED_TEXT_PTR_WIDTH 32

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
  fb_sym.name = function_block.name;
  fb_sym.base_name = fb_sym.name;

  // When calling function blocks, the passed parameters are value-copied to a
  // corresponding instance data block. This block contains all input, inout,
  // output and static variables. The function block reads and writes only
  // those fields and does not modify the actual parameters. To simulate this
  // behaviour, all function blocks are modeled as functions with a single
  // parameter: An instance of their data block, whose members they modify.
  struct_typet data_block = create_instance_data_block(function_block);
  type_symbolt db_sym{data_block};
  db_sym.name = function_block.name + "_db";
  db_sym.base_name = db_sym.name;
  symbol_table.add(db_sym);

  pointer_typet db_ptr{data_block, STRUCTURED_TEXT_PTR_WIDTH};
  code_typet::parameterst params;
  code_typet::parametert param{db_ptr};
  // Just use a generic name for the artificial parameter.
  param.set_identifier("data_block");
  params.push_back(param);
  typet return_type{ID_void};
  fb_sym.type = code_typet{params, return_type};

  fb_sym.value =
    typecheck_structured_text_networks(function_block.networks, param);

  symbol_table.add(fb_sym);
  // TODO: Expand.
}

void structured_text_typecheckt::typecheck_function(
  const structured_text_parse_treet::functiont &function)
{
  // TODO: Implement.
  (void)function;
}

struct_typet structured_text_typecheckt::create_instance_data_block(
  const structured_text_parse_treet::function_blockt &function_block)
{
  struct_union_typet::componentst components;
  typecheck_var_decls(function_block.var_input, components);
  typecheck_var_decls(function_block.var_inout, components);
  typecheck_var_decls(function_block.var_output, components);
  struct_typet result{components};

  return result;
}

void structured_text_typecheckt::typecheck_var_decls(
  const structured_text_parse_treet::var_declarationst &var_decls,
  struct_union_typet::componentst &components)
{
  for(const auto &declaration : var_decls)
  {
    irep_idt var_name = declaration.variable.get_identifier();
    // TODO: Implement actual type check of var types once necessary.
    typet var_type = declaration.variable.type();
    struct_union_typet::componentt component{var_name, var_type};
    components.push_back(component);
  }
}

typet structured_text_typecheckt::typecheck_structured_text_type(
  const typet &standard_text_type)
{
  // TODO: Expand.
  return std::move(standard_text_type);
}

code_blockt structured_text_typecheckt::typecheck_structured_text_networks(
  const structured_text_parse_treet::networkst &networks,
  const code_typet::parametert &fb_vars)
{
  code_blockt result;
  for(const auto &network : networks)
  {
    for(const auto &instruction : network.instructions)
      typecheck_structured_text_instruction(instruction, fb_vars, result);
  }
  return result;
}

void structured_text_typecheckt::typecheck_structured_text_instruction(
  const structured_text_parse_treet::instructiont &instruction,
  const code_typet::parametert &fb_vars,
  code_blockt &block)
{
  // TODO: This might not work in scenarios with multiple codet objects. Expand
  // this section or support only one codet.
  codet op_code = instruction.tokens.back();

  if(op_code.id() == ID_structured_text_load)
    typecheck_structured_text_load(op_code, fb_vars);
  else if(op_code.id() == ID_structured_text_transfer)
    typecheck_structured_text_transfer(op_code, fb_vars, block);
  else if(op_code.id() == ID_structured_text_accu_int_add)
    typecheck_structured_text_accu_int_add();
  else if(op_code.id() == ID_structured_text_nop)
    return;
}

void structured_text_typecheckt::typecheck_structured_text_load(
  const codet &op_code,
  const code_typet::parametert &fb_vars)
{
  // Get identifier of operand.
  irep_idt identifier;
  if(op_code.op0().id() == ID_string_constant)
    identifier = op_code.op0().get(ID_value);
  else
  {
    error() << "Load is not followed by identifier" << eom;
    throw 0;
  }

  // Check if identifier is in data block. If it is, add dereferenced data
  // block member to the accumulator.
  PRECONDITION(can_cast_type<struct_typet>(fb_vars.type().subtype()));
  struct_typet data_block = static_cast<struct_typet>(fb_vars.type().subtype());

  for(const auto &member : data_block.components())
  {
    if(member.get_name() == identifier)
    {
      dereference_exprt deref_db{fb_vars};
      member_exprt val{deref_db, member.get_name(), member.type()};
      accumulator.push_back(std::move(val));
      return;
    }
  }
  error() << "Identifier could not be found in variable declarations" << eom;
  throw 0;
}

void structured_text_typecheckt::typecheck_structured_text_transfer(
  const codet &op_code,
  const code_typet::parametert &fb_vars,
  code_blockt &block)
{
  // Get identifier of operand.
  irep_idt identifier;
  if(op_code.op0().id() == ID_string_constant)
    identifier = op_code.op0().get(ID_value);
  else
  {
    error() << "Transfer is not followed by identifier" << eom;
    throw 0;
  }

  // Check if identifier is in data block. If it is, create code assignment and
  // add it to the function body.
  PRECONDITION(can_cast_type<struct_typet>(fb_vars.type().subtype()));
  struct_typet data_block = static_cast<struct_typet>(fb_vars.type().subtype());

  for(const auto &member : data_block.components())
  {
    if(member.get_name() == identifier)
    {
      dereference_exprt deref_db{fb_vars};
      member_exprt lhs{deref_db, member.get_name(), member.type()};
      // TODO: Add check if assignment is allowed.
      code_assignt assignment(lhs, accumulator.back());
      block.add(assignment);
      return;
    }
  }
  error() << "Identifier could not be found in variable declarations" << eom;
  throw 0;
}

void structured_text_typecheckt::typecheck_structured_text_accu_int_add()
{
  if(accumulator.size() < 2)
  {
    error() << "Not enough operands in the accumulator" << eom;
    throw 0;
  }
  // Pop first operand, peek second.
  exprt &accu1 = accumulator.pop_back();
  exprt &accu2 = accumulator.back();

  // Are both operands members of the data block?
  member_exprt op1;
  member_exprt op2;
  if(can_cast_expr<member_exprt>(accu1) && can_cast_expr<member_exprt>(accu2))
  {
    op1 = to_member_expr(accu1);
    op2 = to_member_expr(accu2);
  }
  else
  {
    error() << "Operands are no members of the data block" << eom;
    throw 0;
  }

  // Are both operands integers? If yes, push plus_exprt to accumulator.
  if(
    op1.get(ID_structured_text_type) == ID_structured_text_int &&
    op2.get(ID_structured_text_type) == ID_structured_text_int)
  {
    plus_exprt operation{op1, op2};
    accumulator.push_back(operation);
  }
  else
  {
    error() << "Operands of integer addition are no integers" << eom;
    throw 0;
  }
}
