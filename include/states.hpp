#pragma once
#include "state.hpp"

namespace Slate::Language 
{
    namespace Detail
    {
        using namespace Meta;

        namespace Detail
        {
            template <typename Type1, typename Type2, typename=void>
            class Valid_Next
            {
            public:
                using Type = Wrap<>;
            };

            template <typename Type1, typename Type2>
            class Valid_Next<Type1, Type2, std::void_t<typename Type1::Next<Type2>>>
            {
            public:
                using Type = typename Type1::Next<Type2>;
            };
        }

        template <typename Type1, typename Type2>
        using Valid_Next = typename Detail::Valid_Next<Type1, Type2>::Type;
        
        template <typename Alphabet, typename Start_State>
        class States
        {};

        template <typename ... Alphabet_Types, typename Start_State>
        class States<Wrap<Alphabet_Types...>, Start_State>
        {
            template <typename T, typename=void>
            class Use_If_Not_Me
            {
            public:
                using Type = typename States<Wrap<Alphabet_Types...>, T>::Type;
            };

            template <typename T>
            class Use_If_Not_Me<T, std::enable_if_t<std::is_same_v<T, Start_State>>>
            {
            public:
                using Type = Wrap<>; 
            };
        public:
            using Type = Join<
                Start_State,
                Valid_Next<Start_State, Alphabet_Types>..., 
                typename Use_If_Not_Me<Valid_Next<Start_State, Alphabet_Types>>::Type...>;
        };

        template <typename T>
        class Terminal_Alphabet
        {
            template <typename C>
            class Builder
            {
            public:
                using Type = Meta::Join_For_Each<C, Terminal_Alphabet>;
            };
        public:
            // W<W<...>, W<...>>
            using Type = Meta::Join_For_Each<Language::Production<T>, Builder>;
        };
        template <Grammar_Terminal T>
        class Terminal_Alphabet<T>
        {
        public:
            using Type = T;
        };

        template <typename T>
        class Token_Alphabet
        {
            template <typename C>
            class Builder
            {
            public:
                using Type = Meta::Join_For_Each<C, Token_Alphabet>;
            };
        public:
            // W<W<...>, W<...>>
            using Type = Meta::Append<T, Meta::Join_For_Each<Language::Production<T>, Builder>>;
        };
        template <Grammar_Terminal T>
        class Token_Alphabet<T>
        {
        public:
            using Type = T;
        };
    }

    template <typename Starting_Grammar>
    using Terminal_Alphabet = Meta::Convert<Meta::Unique<Meta::Append<typename Detail::Terminal_Alphabet<Starting_Grammar>::Type, Terminals::$>>, std::variant>;

    template <typename Starting_Grammar>
    using Token_Alphabet = Meta::Convert<Meta::Unique<Meta::Join<Rules::Start<Starting_Grammar>, typename Detail::Token_Alphabet<Starting_Grammar>::Type, Terminals::$>>, std::variant>;
    
    template <typename Starting_Grammar>
    using State_Alphabet = Meta::Convert<Meta::Unique<typename Detail::States<Meta::Unwrap_Tail<Meta::Convert<Token_Alphabet<Starting_Grammar>>>, State<Rules::Start<Starting_Grammar>>>::Type>, std::variant>;
}