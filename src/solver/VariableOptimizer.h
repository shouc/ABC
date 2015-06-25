/*
 * VariableOptimizer.h
 *
 *  Created on: May 4, 2015
 *      Author: baki
 */

#ifndef SOLVER_VARIABLEOPTIMIZER_H_
#define SOLVER_VARIABLEOPTIMIZER_H_

#include <stack>
#include <map>

#include <glog/logging.h>
#include "../smt/ast.h"
#include "SymbolTable.h"
#include "Counter.h"
#include "OptimizationRuleRunner.h"

namespace Vlab {
namespace Solver {

using namespace SMT;

class VariableOptimizer: public Visitor {
public:
	VariableOptimizer(Script_ptr, SymbolTable_ptr);
	virtual ~VariableOptimizer();
	void start();
	void end();

	void visitScript(Script_ptr);
	void visitCommand(Command_ptr);
	void visitTerm(Term_ptr);
	void visitExclamation(Exclamation_ptr);
	void visitExists(Exists_ptr);
	void visitForAll(ForAll_ptr);
	void visitLet(Let_ptr);
	void visitAnd(And_ptr);
	void visitOr(Or_ptr);
	void visitNot(Not_ptr);
	void visitUMinus(UMinus_ptr);
	void visitMinus(Minus_ptr);
	void visitPlus(Plus_ptr);
	void visitEq(Eq_ptr);
	void visitGt(Gt_ptr);
	void visitGe(Ge_ptr);
	void visitLt(Lt_ptr);
	void visitLe(Le_ptr);
	void visitConcat(Concat_ptr);
	void visitIn(In_ptr);
	void visitLen(Len_ptr);
	void visitContains(Contains_ptr);
	void visitBegins(Begins_ptr);
	void visitEnds(Ends_ptr);
	void visitIndexOf(IndexOf_ptr);
	void visitReplace(Replace_ptr);
	void visitCount(Count_ptr);
	void visitIte(Ite_ptr);
	void visitReConcat(ReConcat_ptr);
	void visitToRegex(ToRegex_ptr);
	void visitUnknownTerm(Unknown_ptr);
	void visitAsQualIdentifier(AsQualIdentifier_ptr);
	void visitQualIdentifier(QualIdentifier_ptr);
	void visitTermConstant(TermConstant_ptr);
	void visitSort(Sort_ptr);
	void visitTVariable(TVariable_ptr);
	void visitTBool(TBool_ptr);
	void visitTInt(TInt_ptr);
	void visitTString(TString_ptr);
	void visitAttribute(Attribute_ptr);
	void visitSortedVar(SortedVar_ptr);
	void visitVarBinding(VarBinding_ptr);
	void visitIdentifier(Identifier_ptr);
	void visitPrimitive(Primitive_ptr);
	void visitVariable(Variable_ptr);
protected:
	void add_variable_substitution_rule(Variable_ptr, Variable_ptr, Term_ptr);
	void add_variable_substitution_rule(Variable_ptr, Term_ptr);

	Script_ptr root;
	SymbolTable_ptr symbol_table;

	Variable::Type target_type;
	bool existential_elimination_phase;
	std::map<Variable_ptr, int> eq_constraint_count;
private:
	static const int VLOG_LEVEL;
};

} /* namespace Solver */
} /* namespace Vlab */

#endif /* SOLVER_VARIABLEOPTIMIZER_H_ */
