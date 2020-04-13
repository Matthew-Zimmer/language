#pragma once
#include <variant>

#include "composition.hpp"
#include "grammar.hpp"

#include <iostream>

template <typename Type>
constexpr bool FALSE = false;

template <typename Type>
void FAIL()
{
    static_assert(FALSE<Type>);
}

namespace Slate::Language
{
    enum class Parsing_State
    {
        accept,
        reject,
        parsing
    };

    namespace Detail
    {
        namespace Detail
        {
            template <typename Type>
            class Is_Start_State : public std::false_type
            {};

            template <typename Type>
            class Is_Start_State<Rules::Start<Type>> : public std::true_type
            {};
        }

        template <typename Type>
        constexpr bool is_start_state = Detail::Is_Start_State<Type>::value;

        using namespace Meta;

        template <typename>
        class State {};

        template <typename Rule, typename ... Rest>
        class State<Wrap<Rule, Rest...>> : public State<Wrap<Rule>>, public State<Wrap<Rest...>>
        {
            using First = State<Wrap<Rule>>;
            using Other = State<Wrap<Rest...>>;
            using This = State<Wrap<Rule, Rest...>>;
        protected:
            using First::next;
            using Other::next;
        public:
            using First::action;
            using Other::action;

            template <typename T>
            using Next = decltype(std::declval<This>().template next<T>());
        };

        template <typename Rule, typename ... Pre, typename F, typename ... Post>
        class State<Wrap<Wrap<Rule, Wrap<Pre...>, Wrap<F, Post...>>>>
        {
            using This = State<Wrap<Wrap<Rule, Wrap<Pre...>, Wrap<F, Post...>>>>;
        protected:
            template <typename T> requires(std::is_same_v<T, F>)
            static State<Slate::Language::Composition<Wrap<Wrap<Rule, Wrap<Pre..., F>, Wrap<Post...>>>>> next();
        public:
            template <typename T>
            using Next = decltype(std::declval<This>().template next<T>());

            template <Grammar_Terminal T> 
            static Parsing_State action(auto& i, auto& s, auto& p) requires(std::is_same_v<T, F>)//shift
            {
                std::cout << "Shift" << std::endl;
                p.push_back(*i++);
                s.push_back(typename This::Next<T>{});
                return Parsing_State::parsing;
            }

            template <Grammar_Rule T> 
            static Parsing_State action(auto& i, auto& s, auto& p) requires(std::is_same_v<T, F>)//goto
            {
                std::cout << "Goto" << std::endl;
                s.push_back(typename This::Next<T>{});
                return Parsing_State::parsing;
            }
        };

        template <typename Rule, typename ... Pre>
        class State<Wrap<Wrap<Rule, Wrap<Pre...>, Wrap<>>>>
        {
            template <typename Iter, typename State_Stack, typename Parse_Stack>
            class Reduce_Visitor
            {
                Iter& i;
                State_Stack& s;
                Parse_Stack& p;
            public:
                Reduce_Visitor(Iter& i, State_Stack& s, Parse_Stack& p) : i{ i }, s{ s }, p{ p } 
                {}
                template <typename T>
                Parsing_State operator()(T) requires(requires(){ T::template action<Rule>(i, s, p); })
                {
                    return T::template action<Rule>(i, s, p);
                }

                template <typename T>
                Parsing_State operator()(T)
                {
                    return Parsing_State::reject;
                }
            };
        public:
            template <typename T> 
            static Parsing_State action(auto& iter, auto& s, auto& p) requires(!Grammar_Rule<T> && !is_start_state<Rule>)//reduce
            {
                std::cout << "Reduce" << std::endl;
                for (std::size_t i{ 0 }; i < sizeof...(Pre);i++)
                {
                    p.pop_back();
                    s.pop_back();
                }
                p.push_back(Rule{});
                return std::visit(Reduce_Visitor<decltype(iter), decltype(s), decltype(p)>{ iter, s, p }, s.back());
            }

            template <typename T> 
            static Parsing_State action(auto& iter, auto& s, auto& p) requires(std::is_same_v<T, Terminals::$> && is_start_state<Rule>)//accept
            {
                std::cout << "Accept" << std::endl;
                return Parsing_State::accept;
            }
        };
    }

    template <typename Grammar>
    using State = Detail::State<Composition<Dot<Grammar>>>;
}