#pragma once

#include "common.hpp"
#include "Symbol.hpp"



namespace intermediate
{

    /// Alias for shared pointer to a symbol
    using SymbolPtr = std::shared_ptr <Symbol>;

    /// Enumerates the scope the symbol table belongs to
    enum class SymbolTableScope
    {
        global,         /// The first symbol table, always existing and only one
        function,       /// Any function
        loop,           /// If loop, while loop
        conditional,    /// Any conditional branches, if, else if, else
        anonymous,      /// A randomly scoped section { code }
    };

    /**
     *  @class : SymbolTable
     *
     *  This class represents a table of symbols in the symbol table stack
     *  The symbols contain information about the scope that this table represents
     *  It is implemented using a map for easy insertion and lookup
     */

    class SymbolTable
    {

    public:

        /**
         *  Constructor
         *  @param scope         : The type of scope this table represents
         *  @param table_name    : Name of the scope this symbol table corresponds to
         *  @param nesting_level : The current nesting level of the symbol table at the time of invokation
         */
        SymbolTable(const SymbolTableScope scope, const std::string table_name, const uint32_t nesting_level) :
            m_scope(scope),
            m_table_name(table_name),
            m_nesting_level(nesting_level),
            m_current_symbol_id(0)
            { /* Empty */}

        /**
         *  Creates a new symbol and adds it into the symbol table
         *  @param name : Name of the symbol
         *  @returns    : A shared pointer to the newly created symbol
         *  @note       : First use of shared_ptr, need to evaluate
         */
        const SymbolPtr & create_and_add_symbol(const std::string & name, backend::TypeSpecifier type);

        /**
         *  Looks up a symbol in the table
         *  @param name : Name of the symbol to look up
         *  @returns    : Pointer to symbol if found, nullptr if not
         */
        SymbolPtr lookup_symbol(const std::string & name) const;

        /// Returns true if a symbol exists in the table
        bool symbol_exists(const std::string & name) const;

        /// Returns the size of the table
        uint32_t get_size() const { return m_table.size(); }

        /// Returns the name of the table
        const std::string & get_table_name() const { return m_table_name; }

        /// Returns the nesting level of the table
        uint32_t get_nesting_level() const { return m_nesting_level; }

        /// Returns the last symbol ID
        uint32_t get_last_symbol_id() const { return m_current_symbol_id - 1; }

        /// Returns a vector of all the symbols in the table
        std::vector <SymbolPtr> glob_all_symbols() const;

        /// Returns a reference to the table data structure
        const std::map <const std::string, SymbolPtr> & get_table() const { return m_table; }

    private:

        /// Scope of this table
        const SymbolTableScope m_scope;

        /// Name of the scope this symbol table corresponds to
        const std::string m_table_name;

        /// The nesting level of the table in the symbol table stack, should never change
        const uint32_t m_nesting_level;

        /// ID of the next symbol to be added
        uint32_t m_current_symbol_id;

        /// Maps symbol name to a pointer to the symbol object
        std::map <const std::string, SymbolPtr> m_table;

    };

} /// intermediate
