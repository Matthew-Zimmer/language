#pragma once
#include "closure.hpp"

namespace Slate::Language::Operators
{
    template <typename Type>
    class Star 
    {
    public:
        template <std::size_t min>
        class At_Least
        {};
        template <std::size_t min>
        class At_Most
        {};
        template <std::size_t min>
        class Exactly
        {};
    };

    template <typename Type>
    using Plus = typename Star<Type>::At_Least<1>;

    template <typename ... Types>
    class Join 
    {
        template <typename>
        class Shift
        {
        public:
            using Head = Meta::Wrap<>;
            using Tail = Meta::Wrap<>;
        };
        template <typename T, typename ... Rest>
        class Shift<Meta::Wrap<T, Rest...>>
        {
        public:
            using Head = Meta::Wrap<T>;
            using Tail = Meta::Wrap<Rest...>;
        };
    public:
        using Closure = Language::Closure<Dot<Meta::Unwrap<Meta::Wrap<Types...>>>>;
        using Shift_Head = typename Shift<Meta::Wrap<Types...>>::Head;
        using Shift_Tail = typename Shift<Meta::Wrap<Types...>>::Tail;
        template <typename T>
        static constexpr bool allowed = std::is_same_v<Meta::Wrap<T>, Shift_Head>;
    };

    template <typename ... Types>
    class Or 
    {
    public:
        using Closure = Meta::Join<Language::Closure<Dot<Types>>...>;
        using Shift_Head = Meta::Wrap<Types...>;
        using Shift_Tail = Meta::Wrap<>;
        template <typename T>
        static constexpr bool allowed = (std::is_same_v<T, Types> || ...);
    };

    template <typename ... Types>
    class Optional
    {};
}