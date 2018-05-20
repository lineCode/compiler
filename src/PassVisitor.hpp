#pragma once

#include <iostream>
#include <set>
#include <map>

#include "wci/intermediate/SymTabStack.h"
#include "wci/intermediate/SymTabEntry.h"
#include "wci/intermediate/TypeSpec.h"

#include "CmmBaseVisitor.h"
#include "antlr4-runtime.h"
#include "CmmVisitor.h"

#include "common.hpp"

using namespace wci;
using namespace wci::intermediate;



/// Common code to all visit nodes, not the best but could not find a better solution
#define PRINT_CONTEXT_AND_EXIT_IF_PARSE_ERROR()                 \
    if (!print_debug_context(context, string(__FUNCTION__)))   \
    {                                                           \
        return nullptr;                                         \
    }

/**
 *  Class that Pass1Visitor and Pass2Visitor inherit from
 *  Contains data structures used by all passes and generic helper functions
 *  Protected constructor to prevent direct instantiation
 */

class PassVisitor
{
protected:

    /// Symbol information / attributes
    typedef struct
    {
        uint32_t id;
        char type_letter;
        TypeSpec * type;
    } symbol_S;

    /// Protected constructor
    PassVisitor(const uint8_t pass_number) : pass_number(pass_number) { }

    /// Virtual destructor
    virtual ~PassVisitor() { }

    /// Enumerates the pass number
    const uint8_t pass_number;

    /// Just a tab character
    static const char TAB = '\t';

    /// Maps string types to a pointer to the typespec
    static const unordered_map <string, TypeSpec **> type_map;

    /// Maps a pointer to the typespec to the type letter
    static const unordered_map <TypeSpec **, char> letter_map;

    /// Maps a pointer to the typespec to the instruction prefix
    static const unordered_map <TypeSpec **, char> instruction_prefix_map;

    /// Maps scope names to maps of symbol names to symbol attributes
    static unordered_map <string, unordered_map <string, PassVisitor::symbol_S>> variable_id_map;

    /// Maps function names to their function invoke signature
    static unordered_map <string, string> function_definition_map;

    /// Stores the current function name
    static string current_function;

    /// Counts up for each compound statement
    static uint64_t scope_counter;

    /**
     *  Determines the resulting type depending on the two operands
     *  @param lhs_type : Type of LHS
     *  @param rhs_type : Type of RHS
     *  @returns        : A single type which is the greater of the two
     */
    TypeSpec * resolve_expression_type(TypeSpec * lhs_type, TypeSpec * rhs_type);

    /**
     *  Looks up the letter with [letter_map]
     *  @param type : The type to lookup
     *  @returns    : The letter looked up, throws if not found
     *  @throws     : InvalidType
     */
    char letter_map_lookup(const TypeSpec * type) const;

    /**
     *  Looks up the letter with [instruction_prefix_map]
     *  @param type : The type to lookup
     *  @returns    : The letter looked up, throws if not found
     *  @throws     : InvalidType
     */
    char instruction_prefix_map_lookup(const TypeSpec * type) const;

    /**
     *  Prints the current visit context information if [debug_flag] is true
     *  @param context   : Current context or parser rule
     *  @param rule_name : Name of current rule
     */
    bool print_debug_context(antlr4::ParserRuleContext * context, const std::string & rule_name) const;

    /**
     *  Wrapper for determining if an identifier is a digit or not
     *  @param identifier : String identifier
     *  @returns          : True for digit
     */
    static bool is_digit(const string & identifier)
    {
        return std::isdigit(identifier[0]);
    }

    /**
     *  Sets up a GET instruction depending on global or not global
     *  @param program_name : The name of the program
     *  @param variable     : The name of the variable
     *  @param type_letter  : The type of the variable
     *  @returns            : The constructed instruction, returns ???? if not found
     *  @throws             : InvalidType if type letter not supported
     */
    string create_get_variable_instruction(const string program_name, const string variable, const char type_letter);

    /**
     *  Sets up a PUT instruction depending on global or not global
     *  @param program_name : The name of the program
     *  @param variable     : The name of the variable
     *  @param type_letter  : The type of the variable
     *  @returns            : The constructed instruction, returns ???? if not found
     *  @throws             : InvalidType if type letter not supported
     */
    string create_put_variable_instruction(const string program_name, const string variable, const char type_letter);

    /**
     *  Looks up the variable ID in the variable id table
     *  @param variable : The name of the variable
     *  @returns        : The enumerated ID of the variable
     */
    uint32_t get_variable_id(const string variable) const;

    /**
     *  Determines if a variable is in the global symbol table
     *  @param variable : The name of the variable
     *  @returns        : True for global, false for not global
     */
    bool is_global(const string variable) const;

    /**
     *  Determines if a type conversion instruction is necessary when working with two types
     *  @param current_type : The type the last instruction was working with
     *  @param needed_type  : The type the next instruction is working with
     *  @returns            : A string with the type instruction, empty if no instruction needed
     */
    string convert_type_if_neccessary(TypeSpec * current_type, TypeSpec * needed_type);

};