#include <iostream>

#include <ordeal/ordeal.hpp>

#include "lexer.hpp"
#include "parser.hpp"
#include "define.hpp"

using namespace Slate::Ordeal;
using namespace Slate::Language;
using namespace Slate::Language::Define;

struct Declaration_Expression : Rule<Keyword<"de">>{};

class Expression;

struct There_Exists : Rule<Join<Keyword<"there_exists">, Declaration_Expression, Expression>>{};

class Expression : public Rule<Or<Declaration_Expression, There_Exists>>
{};

struct Proof : Rule<Expression>{};

class Test_Parser3 : public Unit_Test<Test_Parser3>
{
public:
    Test_Parser3() : Unit_Test{ "test parser3" }
    {}

    auto run(Test<0>) 
    {
        Lexer<Proof> lex;
        LR0<Proof> par;
        std::string input = "there_exists de there_exists de de";
        try 
        {
            auto tokens = lex.tokenize(input);

            // for (auto& token : tokens)
            //     std::visit(Overloaded{
            //         [](auto x){ std::cout << x.str() << std::endl; },
            //         [](Terminals::$){ std::cout << "TERM" << std::endl; }
            //     }, token);

            par.parse(tokens);
            return "parser test:"_name = true;
        }
        catch (std::runtime_error& e)
        {
            return "parser test:"_name = false;
        }
    }
};
