#include <iostream>

#include <ordeal/ordeal.hpp>

#include "lexer.hpp"
#include "parser.hpp"
#include "define.hpp"

using namespace Slate::Ordeal;
using namespace Slate::Language;
using namespace Slate::Language::Define;

struct Expression;

struct Simple : Rule<Keyword<"hello">>
{};

struct There_Exists : Rule<Join<Keyword<"there_exists">, Expression>>
{};

struct Expression : Rule<Or<Simple, There_Exists>>
{};

struct Proof : Rule<Expression> {};

class Test_Parser3 : public Unit_Test<Test_Parser3>
{
public:
    Test_Parser3() : Unit_Test{ "test parser3" }
    {}

    auto run(Test<0>) 
    {
        Lexer<Proof> lex;
        LR0<Proof> par;
        std::string input = "hello";
        try 
        {
            auto tokens = lex.tokenize(input);

            for (auto& token : tokens)
                std::visit(Overloaded{
                    [](auto x){ std::cout << x.str() << std::endl; },
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

    auto run(Test<1>) 
    {
        Lexer<Proof> lex;
        LR0<Proof> par;
        std::string input = "there_exists hello";
        try 
        {
            auto tokens = lex.tokenize(input);

            for (auto& token : tokens)
                std::visit(Overloaded{
                    [](auto x){ std::cout << x.str() << std::endl; },
                    [](Terminals::$){ std::cout << "TERM" << std::endl; }
                }, token);

            par.parse(tokens);
            return "parser test 2:"_name = true;
        }
        catch (std::runtime_error& e)
        {
            return "parser test 2:"_name = false;
        }
    }

    auto run(Test<2>) 
    {
        Lexer<Proof> lex;
        LR0<Proof> par;
        std::string input = "there_exists there_exists hello";
        try 
        {
            auto tokens = lex.tokenize(input);

            for (auto& token : tokens)
                std::visit(Overloaded{
                    [](auto x){ std::cout << x.str() << std::endl; },
                    [](Terminals::$){ std::cout << "TERM" << std::endl; }
                }, token);

            par.parse(tokens);
            return "parser test 3:"_name = true;
        }
        catch (std::runtime_error& e)
        {
            return "parser test 3:"_name = false;
        }
    }
};
