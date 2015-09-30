/*
 * Counter.cpp
 *
 *  Created on: May 4, 2015
 *      Author: baki
 */

#include "Counter.h"

namespace Vlab {
namespace Solver {

using namespace SMT;

const int Counter::VLOG_LEVEL = 17;

Counter::Counter(Script_ptr script, SymbolTable_ptr symbol_table)
        : AstTraverser (script), symbol_table (symbol_table) {
  setCallbacks();
}

Counter::~Counter() {
}

void Counter::start() {
  symbol_table->reset_count();
  symbol_table->push_scope(root);
  visitScript(root);
  symbol_table->pop_scope();
  end();
}

void Counter::end() {
  if (VLOG_IS_ON(VLOG_LEVEL)) {
    for (auto& pair : symbol_table->getVariables()) {
      DVLOG(VLOG_LEVEL) << pair.first << " : " << symbol_table->get_total_count(pair.second);
    }
  }
}

void Counter::setCallbacks() {
  auto term_callback = [this] (Term_ptr term) -> bool {
    switch (term->getType()) {
    case Term::Type::TERMCONSTANT: {
      return false;
    }
    default:
      return true;
    }
  };

  auto command_callback = [](Command_ptr command) -> bool {
    if (Command::Type::ASSERT == command->getType()) {
      return true;
    }
    return false;
  };

  setCommandPreCallback(command_callback);
  setTermPreCallback(term_callback);
}


void Counter::visitOr(Or_ptr or_term) {
  for (auto& term : *(or_term->term_list)) {
    symbol_table->push_scope(term);
    visit(term);
    symbol_table->pop_scope();
  }
}

void Counter::visitQualIdentifier(QualIdentifier_ptr qi_term) {
  symbol_table->increment_count(symbol_table->getVariable(qi_term->getVarName()));
}

} /* namespace Solver */
} /* namespace Vlab */

