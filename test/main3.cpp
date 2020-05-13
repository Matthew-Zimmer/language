#include <iostream>

#include <ordeal/ordeal.hpp>

#include "lexer.hpp"
#include "parser.hpp"
#include "define.hpp"

using namespace Slate::Ordeal;
using namespace Slate::Language;
using namespace Slate::Language::Define;

class Identifier 
{
public:
    static bool is_valid(std::string const& token)
    {
        return true;
    };
};

struct Import : Rule<Join<Keyword<"import">, Identifier, Keyword<".">, Identifier>>{};

struct Using : Rule<Join<Keyword<"using">, Identifier, Keyword<"for">, Identifier>>{};

struct Statement : Rule<Or<Import, Using>>{};

struct Export : Rule<Join<Keyword<"export">, Statement>>{};

struct Library : Rule<Join<Keyword<"library">, Identifier>>{};

struct Proof : Rule<Join<Library, Export>> {};

class Test_Parser3 : public Unit_Test<Test_Parser3>
{
public:
    Test_Parser3() : Unit_Test{ "test parser3" }
    {}

    auto run(Test<0>) 
    {
        Lexer<Proof> lex;
        LR0<Proof> par;
        std::string input = "library test export import test2.num";
        try 
        {
            auto tokens = lex.tokenize(input);

            for (auto& token : tokens)
                std::visit(Overloaded{
                    [](auto x){ std::cout << x.str() << std::endl; },
                    [](Identifier){ std::cout << "ID" << std::endl; },
                    [](Terminals::$){ std::cout << "TERM" << std::endl; }
                }, token);

            par.parse(tokens);
            return "parser test:"_name = true;
        }
        catch (std::runtime_error& e)
        {
            return "parser test:"_name = false;
        }
    }
};
