#pragma once

#include "wci/intermediate/TypeSpec.h"



namespace intermediate
{

    using wci::intermediate::TypeSpec;

    /**
     *  @class : Symbol
     *
     *  The purpose of this class is a completely immutable object
     *  that contains the attributes of a symbol
     */

    class Symbol
    {

    public:

        /**
         *  Constructor
         *  Initializes the constant member variables, and that's it
         *  @TODO : Change to TypeSpecifier
         */
        Symbol(const uint32_t id, const char type_letter, const TypeSpec * type) :
            m_id(id), 
            m_type_letter(type_letter), 
            m_type(type) 
            { /* Empty */ }

        /// Returns the ID of the symbol
        uint32_t get_id() const { return m_id; }

        /// Returns the type letter of the symbol
        char get_type_letter() const { return m_type_letter; }

        /// Returns the type specifier of the symbol
        const TypeSpec * get_type() const { return m_type; }

    private:

        /// ID of the symbol, enumerated in the parent symbol table
        const uint32_t m_id;

        /// The type of the symbol, the Jasmin type specifier
        const char m_type_letter;

        /// Pointer to the Predefined type
        const TypeSpec * m_type;

        /// Keeps track of the line numbers this symbol was referenced
        std::vector <uint32_t> line_numbers;

    };

} /// intermediate