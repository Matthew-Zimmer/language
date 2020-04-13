#pragma once
#include <stdexcept>
#include "states.hpp"

template<class... Ts> 
class Overloaded : public Ts... 
{
public:
    using Ts::operator()...; 
};
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

namespace Slate::Language
{
    template <typename Iter, typename State_Stack, typename Parse_Stack>
    class State_Visitor
    {
        Iter& i;
        State_Stack& s;
        Parse_Stack& p;
    public:
        State_Visitor(Iter& i, State_Stack& s, Parse_Stack& p) : i{ i }, s{ s }, p{ p } 
        {}
        template <typename Type1, typename Type2>
        Parsing_State operator()(Type1, Type2)
        {
            if constexpr (requires(){Type1::template action<Type2>(i, s, p); })
                return Type1::template action<Type2>(i, s, p);
            else
                return Parsing_State::reject;
        }
    };

    template <typename Starting_Grammar>
    class LR0
    {
        using Principle_Rule = Starting_Grammar;
        using Augmented_State = State<Rules::Start<Starting_Grammar>>;
        using Terminals = Terminal_Alphabet<Starting_Grammar>;
        using Tokens = Token_Alphabet<Starting_Grammar>;
        using States = State_Alphabet<Starting_Grammar>;
    public:
        std::vector<States> state_stack;
        std::vector<Tokens> parse_stack;
        std::vector<Principle_Rule> data;

        auto initial_state()
        {
            return Augmented_State{};
        }

        void add_parsed_entry(Tokens const& token)
        {
            std::visit(Overloaded{
                [&](auto const&){},
                [&](Principle_Rule const& t){ data.push_back(t); }
            }, token);
        }
        
        std::vector<Tokens> upgrade(std::vector<Terminals> const& t)
        {
            std::vector<Tokens> r;
            for (auto& x : t)
                std::visit([&](auto x){ r.push_back(Tokens{x}); }, x);
            return r;
        }

        auto parse(std::vector<Terminal_Alphabet<Starting_Grammar>> const& terminals)
        {
            auto d = upgrade(terminals);
            auto iter = d.begin();
            state_stack.emplace_back(initial_state());
            State_Visitor visitor{ iter, state_stack, parse_stack };
            while (true)
                if (auto state = std::visit(visitor, state_stack.back(), *iter); state == Parsing_State::accept)
                {
                    add_parsed_entry(parse_stack.back());
                    break;
                }
                else if (state == Parsing_State::reject)
                {
                    throw std::runtime_error{ "Invalid code" };
                }
                {}
            return data;
        }
    };

    template <typename Type>
    class SLR
    {};

    template <typename Type>
    class CLR
    {};

    template <typename Type>
    class LALR
    {};
}