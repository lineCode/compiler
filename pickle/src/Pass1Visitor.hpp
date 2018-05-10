#ifndef PASS1VISITOR_H_
#define PASS1VISITOR_H_

#include <iostream>

#include "wci/intermediate/SymTabStack.h"
#include "wci/intermediate/SymTabEntry.h"
#include "wci/intermediate/TypeSpec.h"

#include "Pcl2BaseVisitor.h"
#include "antlr4-runtime.h"
#include "Pcl2Visitor.h"

using namespace wci;
using namespace wci::intermediate;

class Pass1Visitor : public Pcl2BaseVisitor
{
private:

    SymTabStack *symtab_stack;
    SymTabEntry *program_id;
    vector <SymTabEntry *> variable_id_list;
    ofstream j_file;
    const bool debug_flag;
    TypeSpec * current_type;

    string variable_declaration;

    bool has_initializer;

    bool next_declaration_is_function;

    /// Prints the current visit context information if [debug_flag] is true
    void print_debug_context(const std::string & msg) const;

    TypeSpec * set_expression_type(TypeSpec * lhs_type, TypeSpec * rhs_type);

public:

    /// Constructor
    Pass1Visitor(const bool debug=false);
    
    /// Destructor
    virtual ~Pass1Visitor();

    /// Returns the output file
    ofstream & get_assembly_file();

    /// @ { Compilation Unit
    antlrcpp::Any visitCompilationUnit(Pcl2Parser::CompilationUnitContext * context) override;
    antlrcpp::Any visitTranslationUnit(Pcl2Parser::TranslationUnitContext * context) override;
    /// @ }

    /// @ { Type Specifiers
    antlrcpp::Any visitVoidType(Pcl2Parser::VoidTypeContext * context) override;
    antlrcpp::Any visitBoolType(Pcl2Parser::BoolTypeContext * context) override;
    antlrcpp::Any visitCharType(Pcl2Parser::CharTypeContext * context) override;
    antlrcpp::Any visitShortType(Pcl2Parser::ShortTypeContext * context) override;
    antlrcpp::Any visitIntType(Pcl2Parser::IntTypeContext * context) override;
    antlrcpp::Any visitLongType(Pcl2Parser::LongTypeContext * context) override;
    antlrcpp::Any visitFloatType(Pcl2Parser::FloatTypeContext * context) override;
    antlrcpp::Any visitDoubleType(Pcl2Parser::DoubleTypeContext * context) override;
    antlrcpp::Any visitSignedType(Pcl2Parser::SignedTypeContext * context) override;
    antlrcpp::Any visitUnsignedType(Pcl2Parser::UnsignedTypeContext * context) override;
    antlrcpp::Any visitUint32_tType(Pcl2Parser::Uint32_tTypeContext * context) override;
    antlrcpp::Any visitInt32_tType(Pcl2Parser::Int32_tTypeContext * context) override;
    /// @ }

    /// @ { Declarations
    antlrcpp::Any visitInitializer(Pcl2Parser::InitializerContext *context) override;
    antlrcpp::Any visitDeclaration(Pcl2Parser::DeclarationContext * context) override;
    antlrcpp::Any visitDeclarationSpecifiers(Pcl2Parser::DeclarationSpecifiersContext * context) override;
    antlrcpp::Any visitDeclarationSpecifier(Pcl2Parser::DeclarationSpecifierContext * context) override;
    antlrcpp::Any visitInitDeclaratorList(Pcl2Parser::InitDeclaratorListContext * context) override;
    antlrcpp::Any visitInitDeclarator(Pcl2Parser::InitDeclaratorContext * context) override;
    antlrcpp::Any visitDirectDeclarator(Pcl2Parser::DirectDeclaratorContext * context) override;
    antlrcpp::Any visitDeclarator(Pcl2Parser::DeclaratorContext * context) override;
    antlrcpp::Any visitExternalDeclaration(Pcl2Parser::ExternalDeclarationContext * context) override;
    antlrcpp::Any visitParameterDeclaration(Pcl2Parser::ParameterDeclarationContext * context) override;
    antlrcpp::Any visitFunctionDefinition(Pcl2Parser::FunctionDefinitionContext * context) override;
    /// @ }

    /// @ { Lists
    antlrcpp::Any visitParameterTypeList(Pcl2Parser::ParameterTypeListContext * context) override;
    antlrcpp::Any visitParameterList(Pcl2Parser::ParameterListContext * context) override;
    antlrcpp::Any visitIdentifierList(Pcl2Parser::IdentifierListContext * context) override;
    antlrcpp::Any visitBlockItemList(Pcl2Parser::BlockItemListContext * context) override;
    antlrcpp::Any visitBlockItem(Pcl2Parser::BlockItemContext * context) override;
    /// @ }

    /// @ { Expressions
    antlrcpp::Any visitMultiplicativeExpression(Pcl2Parser::MultiplicativeExpressionContext *context) override;
    antlrcpp::Any visitAdditiveExpression(Pcl2Parser::AdditiveExpressionContext *context) override;
    antlrcpp::Any visitShiftExpression(Pcl2Parser::ShiftExpressionContext *context) override;
    antlrcpp::Any visitRelationalExpression(Pcl2Parser::RelationalExpressionContext *context) override;
    antlrcpp::Any visitEqualityExpression(Pcl2Parser::EqualityExpressionContext *context) override;
    antlrcpp::Any visitAndExpression(Pcl2Parser::AndExpressionContext *context) override;
    antlrcpp::Any visitExclusiveOrExpression(Pcl2Parser::ExclusiveOrExpressionContext *context) override;
    antlrcpp::Any visitInclusiveOrExpression(Pcl2Parser::InclusiveOrExpressionContext *context) override;
    antlrcpp::Any visitLogicalAndExpression(Pcl2Parser::LogicalAndExpressionContext *context) override;
    antlrcpp::Any visitLogicalOrExpression(Pcl2Parser::LogicalOrExpressionContext *context) override;
    antlrcpp::Any visitConditionalExpression(Pcl2Parser::ConditionalExpressionContext *context) override;
    antlrcpp::Any visitAssignmentExpression(Pcl2Parser::AssignmentExpressionContext *context) override;
    antlrcpp::Any visitExpression(Pcl2Parser::ExpressionContext *context) override;
    antlrcpp::Any visitForExpression(Pcl2Parser::ForExpressionContext *context) override;
    /// @ }
};

#endif /* PASS1VISITOR_H_ */
