#pragma once
#include "production.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        template <typename>
        class Dot
        {
        public:
            using Type = Meta::Wrap<>;
        };

        template <Grammar_Rule T>
        class Dot<T>
        {
        public:
            using Type = Meta::Wrap<T, Meta::Wrap<>, Production<T>>;   
        };
    }

    template <typename Type>
    using Dot = typename Detail::Dot<Type>::Type;
}