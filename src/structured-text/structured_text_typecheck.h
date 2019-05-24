/*******************************************************************\

Module: Structured Text Language Type Checking

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Type Checking

#ifndef CPROVER_STRUCTURED_TEXT_STRUCTURED_TEXT_TYPECHECK_H
#define CPROVER_STRUCTURED_TEXT_STRUCTURED_TEXT_TYPECHECK_H

#include <util/std_code.h>
#include <util/symbol_table.h>
#include <util/typecheck.h>

#include "structured_text_parse_tree.h"

/// Create a new structured_text_typecheckt object and perform a type check to
/// fill the symbol table.
/// \param parse_tree: Parse tree generated by parsing a Structured Text file.
/// \param [out] symbol_table: Object that shall be filled by this function.
///   If the symbol table is not empty when calling this function, its contents
///   are merged with the new entries.
/// \param module: Name of the file that has been parsed.
/// \param message_handler: Used to provide debug information and error
///   messages.
/// \return: False if no errors occurred, true otherwise.
bool structured_text_typecheck(
  structured_text_parse_treet &parse_tree,
  symbol_tablet &symbol_table,
  const std::string &module,
  message_handlert &message_handler);

/// Class for encapsulating the current state of the type check.
class structured_text_typecheckt : public typecheckt
{
public:
  structured_text_typecheckt(
    structured_text_parse_treet &parse_tree,
    symbol_tablet &symbol_table,
    const std::string &module,
    message_handlert &message_handler)
    : typecheckt(message_handler),
      parse_tree(parse_tree),
      symbol_table(symbol_table),
      module(module)
  {
  }

  void typecheck() override;
  void
  typecheck_function(const structured_text_parse_treet::functiont &function);
  void typecheck_function_block(
    const structured_text_parse_treet::function_blockt &function_block);

private:
  structured_text_parse_treet &parse_tree;
  symbol_tablet &symbol_table;
  const irep_idt module;

  // Internal state of the PLC program.
  std::vector<exprt> accumulator;
  // TODO: Implement status words etc.

  // High level checks

  struct_typet create_instance_data_block(
    const structured_text_parse_treet::function_blockt &function_block);
  void typecheck_var_decls(
    const structured_text_parse_treet::var_declarationst &var_decls,
    struct_union_typet::componentst &components);
  typet typecheck_structured_text_type(const typet &standard_text_type);
  code_blockt typecheck_structured_text_networks(
    const structured_text_parse_treet::networkst &networks,
    const code_typet::parametert &fb_vars);
  void typecheck_structured_text_instruction(
    const structured_text_parse_treet::instructiont &instruction,
    const code_typet::parametert &fb_vars,
    code_blockt &block);

  // Load and Transfer instructions

  void typecheck_structured_text_load(
    const codet &op_code,
    const code_typet::parametert &fb_vars);
  void typecheck_structured_text_transfer(
    const codet &op_code,
    const code_typet::parametert &fb_vars,
    code_blockt &block);

  // Arithmetic accumulator instructions

  void typecheck_structured_text_accu_int_add();
};

#endif // CPROVER_STRUCTURED_TEXT_STRUCTURED_TEXT_TYPECHECK_H
