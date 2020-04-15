// #include "lexer.hpp"
// #include "parser.hpp"
// #include "operators.hpp"

// using namespace Slate::Language;
// using namespace Slate::Meta;
// using namespace Slate::Language::Operators;

// class B
// {
// public:
//     static bool is_valid(std::string const& s){ return s == "b"; }
// };

// class E
// {
// public:
//     static bool is_valid(std::string const& s){ return s == "e"; }
// };

// class C
// {
// public:
//     using Production = B;
// };

// class A
// {
// public:
//     using Production = Or<C, E>;
// };



// using Dot_A = Dot<A>;

// void test_dot_operation()
// {
//     static_assert(std::is_same_v<Dot_A, 
//         Wrap<Wrap<A, Wrap<>, Wrap<Or<C, E>>>>, 
//     "Dot operation incorrect");
// }

// using Composition_A = Composition<Dot_A>;

// void test_composition_operation()
// {
//     static_assert(std::is_same_v<Composition_A, 
//         Wrap<Wrap<A, Wrap<>, Wrap<C>>, 
//              Wrap<C, Wrap<>, Wrap<B>>,
//              Wrap<A, Wrap<>, Wrap<E>>>>, 
//     "Composition operation incorrect");
// }

// using State_A = State<A>;

// void test_state_operation()
// {
//     static_assert(requires(){
//         typename State_A::Next;
//     }, "State A does not have the C transition");
// }

// void test_states_operation()
// {
//     static_assert(std::is_same_v<Token_Alphabet<A>, std::variant<Rules::Start<A>, A, C, B, E, Terminals::$>>, "Wrong token alphabet");
//     // static_assert(std::is_same_v<State_Alphabet<A>, std::variant<
//     //     Slate::Language::Detail::State<Wrap<Wrap<Rules::Start<A>, Wrap<>, Wrap<A>>, Wrap<A, Wrap<>, Wrap<C>>, Wrap<C, Wrap<>, Wrap<B>>, Wrap<A, Wrap<>, Wrap<E>>>>,
//     //     Slate::Language::Detail::State<Wrap<Wrap<Rules::Start<A>, Wrap<A>, Wrap<>>>>,
//     //     Slate::Language::Detail::State<Wrap<Wrap<C, Wrap<B>, Wrap<>>>>,
//     //     Slate::Language::Detail::State<Wrap<Wrap<A, Wrap<C>, Wrap<>>>>,
//     //     Slate::Language::Detail::State<Wrap<Wrap<A, Wrap<E>, Wrap<>>>>
//     //     >>, "states alphabet is wrong");
// }

// void test_lexer()
// {
//     Lexer<A> lex;
//     std::string input = "";
//     lex.tokenize(input);
// }

// #include <ordeal/ordeal.hpp>
// #include <iostream>

// using namespace Slate::Ordeal;

// class Test_Parser : public Unit_Test<Test_Parser>
// {
// public:
//     Test_Parser() : Unit_Test{ "test parser" }
//     {}

//     auto run(Test<0>) {
//         LR0<A> par;
//         Lexer<A> lex;
//         std::string input = "b";
//         try 
//         {
//             auto tokens = lex.tokenize(input);

//             for(auto& token : tokens)
//                 std::visit(Overloaded{
//                     [](A){ std::cout << "A" << std::endl; },
//                     [](B){ std::cout << "B" << std::endl; },
//                     [](C){ std::cout << "C" << std::endl; },
//                     [](E){ std::cout << "E" << std::endl; },
//                     [](Terminals::$){ std::cout << "TERM" << std::endl; },
//                     [](Rules::Start<A>){ std::cout << "START" << std::endl; }
//                 }, token);

//             par.parse(tokens);
//             return "parser test: b"_name = true;
//         }
//         catch (std::runtime_error& e)
//         {
//             return "parser test: b"_name = false;
//         }
//     }

//     auto run(Test<1>) {
//         LR0<A> par;
//         Lexer<A> lex;
//         std::string input = "e";
//         try 
//         {
//             auto tokens = lex.tokenize(input);

//             for(auto& token : tokens)
//                 std::visit(Overloaded{
//                     [](A){ std::cout << "A" << std::endl; },
//                     [](B){ std::cout << "B" << std::endl; },
//                     [](C){ std::cout << "C" << std::endl; },
//                     [](E){ std::cout << "E" << std::endl; },
//                     [](Terminals::$){ std::cout << "TERM" << std::endl; },
//                     [](Rules::Start<A>){ std::cout << "START" << std::endl; }
//                 }, token);

//             par.parse(tokens);
//             return "parser test: e"_name = true;
//         }
//         catch (std::runtime_error& e)
//         {
//             return "parser test: e"_name = false;
//         }
//     }
// };
