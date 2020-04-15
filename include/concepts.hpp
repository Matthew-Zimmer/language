#pragma once
#include <string>

#include <meta/meta.hpp>

namespace Slate::Language
{
    template <typename Type>
    concept Grammar_Rule = requires()
    {
        typename Type::Production;
    };

    template <typename Type>
    concept Grammar_Terminal = requires(std::string s)
    {
        { Type::is_valid(s) } -> std::same_as<bool>;
    };

    template <typename Type>
    concept Grammar_Operator = requires()
    {
        typename Type::Closure;
        typename Type::Shift_Head;
        typename Type::Shift_Tail;
    };
}