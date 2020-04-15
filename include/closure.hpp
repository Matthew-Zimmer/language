#pragma once
#include "dot.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        template <typename>
        class Closure
        {
        public:
            using Type = Meta::Wrap<>;
        };

        template <typename Rule, typename ... Pre, typename ... Post>
        class Closure<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<Post...>>>
        {
        public:
            using Type = Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<Post...>>>;
        };

        template <typename Rule, typename ... Pre, Grammar_Rule Next, typename ... Post>
        class Closure<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<Next, Post...>>>
        {
        public:
            using Type = Meta::Append<Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<Next, Post...>>>, typename Closure<Language::Dot<Next>>::Type>;
        };

        template <typename Rule, typename ... Pre, Grammar_Operator Next, typename ... Post>
        class Closure<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<Next, Post...>>>
        {
        public:
            using Type = Meta::Append<Meta::Wrap<Meta::Wrap<Rule, Meta::Wrap<Pre...>, Meta::Wrap<Next, Post...>>>, typename Next::Closure>;
        };
    }

    template <typename T>
    using Closure = typename Detail::Closure<T>::Type;
}