#pragma once
#include "concepts.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        template <typename>
        class Next{};

        template <typename Rule, typename ... Pre, typename F, typename ... Post>
        class Next<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>>
        {
        public:
            using Type = Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre..., F>, Meta::Wrap<Post...>>>;
        };
        
        template <typename Rule, typename ... Pre, Grammar_Operator F, typename ... Post>
        class Next<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>>
        {
            template <typename U, typename V>
            class Expand
            {
            public:
                using Type = Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre..., U>, Meta::Wrap<V, Post...>>>;
            };
            template <typename ... Us, typename V>
            class Expand<Meta::Wrap<Us...>, V>
            {
            public:
                using Type = Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre..., Us>, Meta::Join<V, Post...>>...>;
            };
        public:
            using Type = typename Expand<typename F::Shift_Head, typename F::Shift_Tail>::Type;
        };

        template <typename>
        class Next_Token
        {};

        template <typename Rule, typename Pre, typename Post>
        class Next_Token<Meta::Wrap<Rule, Pre, Post>>
        {
            template <typename, typename=void>
            class Extract{};
        public:
            using Type = Meta::Wrap<F>;
        };

        template <typename, typename>
        class Next_For{};

        template <typename Rule, typename ... Pre, typename F, typename ... Post, typename T>
        class Next_For<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>, T>
        {
        public:
            using Type = Meta::Wrap<Rule, Meta::Wrap<Pre..., F>, Meta::Wrap<Post...>>;
        };
        
        template <typename Rule, typename ... Pre, Grammar_Operator F, typename ... Post, typename T>
        class Next_For<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<F, Post...>>, T>
        {
        public:
            using Type = Meta::Wrap<Rule, Meta::Wrap<Pre..., T>, Meta::Join<typename F::Shift_Tail, Post...>>;
        };

        template <typename T, typename F>
        class Allowed_Transition : public std::is_same<T, F>
        {};

        template <typename T, Grammar_Operator F>
        class Allowed_Transition<T, F>
        {
        public:
            static constexpr bool value = F::template allowed<T>;
        };
    }

    template <typename Type>
    using Next = typename Detail::Next<Type>::Type;

    template <typename T, typename F>
    constexpr bool allowed_transition = Detail::Allowed_Transition<T, F>::value;
}