#pragma once
#include <variant>

#include "closure.hpp"
#include "grammar.hpp"
#include "next.hpp"

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

        template <typename>
        class State {};

        template <typename ... Rules>
        class State<Meta::Wrap<Rules...>> : public State<Meta::Wrap<Rules>>...
        {
        public:
            using State<Meta::Wrap<Rules>>::action...;
            using Next = Meta::Unique<Meta::Join<typename State<Meta::Wrap<Rules>>::Next...>>;
            using X = void;
        };

        template <typename Rule, typename ... Pre, typename F, typename ... Post>
        class State<Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>>>
        {
            template <typename T>
            class Builder
            {
            public:
                using Type = State<Language::Closure<T>>;
            };
        public:
            using Next = Meta::Join_For_Each<Language::Next<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>>, Builder>;

            template <Grammar_Terminal T> 
            static Parsing_State action(auto& i, auto& s, auto& p) requires(allowed_transition<T, F>)// shift
            {
                std::cout << "Shift" << std::endl;
                p.push_back(*i++);
                s.push_back(typename Builder<typename Next_For<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>, T>::Type>::Type{});
                return Parsing_State::parsing;
            }

            template <Grammar_Rule T> 
            static Parsing_State action(auto& i, auto& s, auto& p) requires(allowed_transition<T, F>)// goto
            {
                std::cout << "Goto" << std::endl;
                s.push_back(typename Builder<typename Next_For<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>, T>::Type>::Type{});
                return Parsing_State::parsing;
            }
        };

        template <typename Rule, typename ... Pre>
        class State<Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<>>>>
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
                    std::cout << "error state" << std::endl;
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

            using Next = Meta::Wrap<>;
        };
    }

    template <typename Grammar>
    using State = Detail::State<Closure<Dot<Grammar>>>;
}