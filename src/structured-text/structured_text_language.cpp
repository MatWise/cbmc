/*******************************************************************\

Module: Structured Text Language Interface

Author: Matthias Weiss, matthias.weiss@diffblue.com

\*******************************************************************/

/// \file
/// Structured Text Language Interface

#include "structured_text_language.h"
#include "converters/expr2structured_text.h"
#include "structured_text_parser.h"
#include "structured_text_typecheck.h"

#include <linking/linking.h>
#include <linking/remove_internal_symbols.h>
#include <util/get_base_name.h>

void structured_text_languaget::set_language_options(const optionst &options)
{
  (void)options;
  return;
}

bool structured_text_languaget::generate_support_functions(
  symbol_tablet &symbol_table)
{
  return false;
}

bool structured_text_languaget::typecheck(
  symbol_tablet &symbol_table,
  const std::string &module,
  const bool keep_file_local)
{
  symbol_tablet new_symbol_table;

  if(structured_text_typecheck(
       parse_tree, new_symbol_table, module, get_message_handler()))
  {
    return true;
  }

  remove_internal_symbols(
    new_symbol_table, this->get_message_handler(), keep_file_local);

  if(linking(symbol_table, new_symbol_table, get_message_handler()))
    return true;

  return false;
}

bool structured_text_languaget::parse(
  std::istream &instream,
  const std::string &path)
{
  structured_text_parser.clear();
  parse_path = path;
  structured_text_parser.set_line_no(0);
  structured_text_parser.set_file(path);
  structured_text_parser.in = &instream;
  structured_text_scanner_init();
  bool result = structured_text_parser.parse();

  // store result
  structured_text_parser.swap_tree(parse_tree);

  return result;
}

void structured_text_languaget::show_parse(std::ostream &out)
{
  parse_tree.output(out);
}

bool structured_text_languaget::can_keep_file_local()
{
  return true;
}

bool structured_text_languaget::typecheck(
  symbol_tablet &symbol_table,
  const std::string &module)
{
  return typecheck(symbol_table, module, true);
}

bool structured_text_languaget::from_expr(
  const exprt &expr,
  std::string &code,
  const namespacet &ns)
{
  code = expr2stl(expr);
  return false;
}

bool structured_text_languaget::from_type(
  const typet &type,
  std::string &code,
  const namespacet &ns)
{
  code = type2stl(type);
  return false;
}

bool structured_text_languaget::type_to_name(
  const typet &type,
  std::string &name,
  const namespacet &ns)
{
  return from_type(type, name, ns);
}

bool structured_text_languaget::to_expr(
  const std::string &code,
  const std::string &module,
  exprt &expr,
  const namespacet &ns)
{
  // unused parameters
  (void)code;
  (void)module;
  (void)expr;
  (void)ns;

  return true;
}

structured_text_languaget::structured_text_languaget()
{
}

structured_text_languaget::~structured_text_languaget()
{
  parse_tree.clear();
}

void structured_text_languaget::modules_provided(std::set<std::string> &modules)
{
  modules.insert(get_base_name(parse_path, true));
}

std::set<std::string> structured_text_languaget::extensions() const
{
  return {"awl"};
}

std::unique_ptr<languaget> new_structured_text_language()
{
  return util_make_unique<structured_text_languaget>();
}

std::unique_ptr<languaget> structured_text_languaget::new_language()
{
  return util_make_unique<structured_text_languaget>();
}

std::string structured_text_languaget::id() const
{
  return "Structured Text";
}

std::string structured_text_languaget::description() const
{
  return "Structured Text Language by Siemens";
}
