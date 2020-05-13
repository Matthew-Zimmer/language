#include <iostream>

#include <ordeal/ordeal.hpp>

#include "lexer.hpp"
#include "parser.hpp"
#include "define.hpp"

using namespace Slate::Ordeal;
using namespace Slate::Language;
using namespace Slate::Language::Define;

struct a : Keyword<"a">{};
struct b : Keyword<"b">{};
struct c : Keyword<"c">{};

struct A : Rule<Join<a, b>>{};
struct B : Rule<Or<Join<a, c>, A>>{};

// class Test_Parser2 : public Unit_Test<Test_Parser2>
// {
// public:
//     Test_Parser2() : Unit_Test{ "test parser2" }
//     {}

//     auto run(Test<0>) 
//     {
//         Lexer<B> lex;
//         LR0<B> par;
//         std::string input = "a b";
//         try 
//         {
//             auto tokens = lex.tokenize(input);

//             for (auto& token : tokens)
//                 std::visit(Overloaded{
//                     [](auto x){ std::cout << x.str() << std::endl; },
//                     [](Terminals::$){ std::cout << "TERM" << std::endl; }
//                 }, token);

//             par.parse(tokens);
//             return "parser test:"_name = true;
//         }
//         catch (std::runtime_error& e)
//         {
//             return "parser test:"_name = false;
//         }
//     }
// };