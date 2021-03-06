#pragma once
#include <functional>
#include <memory>
#include <boost/noncopyable.hpp>
#include <libdevcore/Common.h>
#include <libevmasm/SourceLocation.h>
#include <libsolidity/ast/ASTVisitor.h>
#include <libsolidity/codegen/LValue.h>
#include <libsolidity/interface/Exceptions.h>

namespace dev {
namespace solidity {

/**
 * Compiler for Lity rule engine
 */
class RuleEngineCompiler: private ASTConstVisitor
{
public:
	explicit RuleEngineCompiler(CompilerContext& _compilerContext): m_context(_compilerContext) {}

	/// Appends inline code to fire all rules
	void appendFireAllRules(ContractDefinition const& _contract);

	/// Insert a fact into working memory.
	/// stack pre: fact
	/// stack post: factID
	/// @param factType the type of the fact
	void appendFactInsert(TypePointer const& factType);

	/// Delete a fact from working memory.
	/// stack pre: factID
	/// stack post:
	void appendFactDelete();


	bool visit(Rule const& _node) override;
	bool visit(FactDeclaration const& _node) override;
	bool visit(FieldExpression const& _node) override;
	bool visit(FunctionDefinition const&) override { return false; }
	bool visit(Block const& _function) override;

	void endVisit(Rule const&) override;
	void endVisit(FactDeclaration const&) override;
	void endVisit(FieldExpression const&) override;

	CompilerUtils utils();
private:

	void appendPushItemToStorageArray(h256 itemAddr);
	void appendAccessIndexStorage();

	const Rule* m_currentRule;
	const FactDeclaration* m_currentFact;
	int m_currentFieldNo=0;

	std::vector<dev::u256> m_nodeOutListAddr;

	CompilerContext& m_context;
};

}
}
