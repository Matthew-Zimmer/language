#pragma once
#include "production.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        using namespace Meta;

        template <typename Rule>
        class Dot_Operator
        {
        public:
            template <typename T>
            class Dot_Expansion
            {
            public:
                using Type = Wrap<Rule, Wrap<>, T>;
            };
        };
    }

    template <Grammar_Rule Type>
    using Dot = Meta::For_Each<Production<Type>, Detail::Dot_Operator<Type>::template Dot_Expansion>;  
}