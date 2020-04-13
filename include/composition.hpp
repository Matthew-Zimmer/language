#pragma once
#include "dot.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        using namespace Meta;

        template <typename>
        class Composition
        {};

        template <typename Rule, typename ... Pre, Slate::Language::Grammar_Rule F, typename ... Post>
        class Composition<Wrap<Rule, Wrap<Pre...>, Wrap<F, Post...>>>
        {
        public:
            using Type = Join<Wrap<Wrap<Rule, Wrap<Pre...>, Wrap<F, Post...>>>, Join_For_Each<Dot<F>, Composition>>;
        };

        template <typename Rule, typename ... Pre, typename F, typename ... Post>
        class Composition<Wrap<Rule, Wrap<Pre...>, Wrap<F, Post...>>>
        {
        public:
            using Type = Wrap<Wrap<Rule, Wrap<Pre...>, Wrap<F, Post...>>>;
        };

        template <typename Rule, typename ... Pre>
        class Composition<Wrap<Rule, Wrap<Pre...>, Wrap<>>>
        {
        public:
            using Type = Wrap<Wrap<Rule, Wrap<Pre...>, Wrap<>>>;
        };
    }

    template <typename Type>
    using Composition = Meta::Join_For_Each<Type, Detail::Composition>;
}