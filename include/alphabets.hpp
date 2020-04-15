#pragma once
#include "state.hpp"

namespace Slate::Language 
{
    namespace Detail
    {
        // namespace Detail
        // {
        //     template <typename Type1, typename Type2, typename=void>
        //     class Valid_Next
        //     {
        //     public:
        //         using Type = Meta::Wrap<>;
        //     };

        //     template <typename Type1, typename Type2>
        //     class Valid_Next<Type1, Type2, std::void_t<typename Type1::Next>>
        //     {
        //     public:
        //         using Type = typename Type1::Next<Type2>;
        //     };
        // }

        // template <typename Type1, typename Type2>
        // using Valid_Next = typename Detail::Valid_Next<Type1, Type2>::Type;

        template <typename S, typename V = Meta::Wrap<>>
        class States
        {
            template <typename T, typename=void>
            class Expand
            {
            public:
                using Type = Meta::Wrap<>;
            };
            template <typename T>
            class Expand<T, std::enable_if_t<!Meta::contains<V, T>>>
            {
            public:
                using Type = typename States<T, Meta::Append<T, V>>::Type;
            };
        public:
            using Type = Meta::Append<S, Meta::Join_For_Each<typename S::Next, Expand>>;
        };

        template <typename T, typename K = Meta::Wrap<>>
        class Full_Alphabet
        {
            template <typename U, typename=void>
            class Expand
            {
            public:
                using Type = Meta::Wrap<>;
            };
            template <typename U>
            class Expand<U, std::enable_if_t<!Meta::contains<K, U>>>
            {
            public:
                using Type = typename Full_Alphabet<U, K>::Type;
            };
        public:
            using Type = Meta::Join_For_Each<Meta::Convert<T>, Expand>;
        };
        template <Grammar_Rule T, typename K>
        class Full_Alphabet<T, K>
        {
        public:
            using Type = Meta::Append<T, typename Full_Alphabet<Language::Production<T>, Meta::Append<T, K>>::Type>;
        };
        template <Grammar_Terminal T, typename K>
        class Full_Alphabet<T, K>
        {
        public:
            using Type = T;
        };
        template <typename T>
        using Filter_Terminals = Meta::Use_If<Grammar_Terminal<T>, T>;
    }

    template <typename Starting_Grammar>
    using Terminal_Alphabet = Meta::Convert<Meta::Unique<Meta::Join_For_Each<Meta::Append<typename Detail::Full_Alphabet<Starting_Grammar>::Type, Terminals::$>, Detail::Filter_Terminals>>, std::variant>;

    template <typename Starting_Grammar>
    using Token_Alphabet = Meta::Convert<Meta::Unique<Meta::Join<Rules::Start<Starting_Grammar>, typename Detail::Full_Alphabet<Starting_Grammar>::Type, Terminals::$>>, std::variant>;
    
    template <typename Starting_Grammar>
    using State_Alphabet = Meta::Convert<Meta::Unique<typename Detail::States<State<Rules::Start<Starting_Grammar>>>::Type>, std::variant>;
}