#pragma once
#include "concepts.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        template <typename T>
        class Production
        {
        public:
            using Type = Meta::Wrap<Meta::Wrap<T>>;
        };

        template <Grammar_Expansion T>
        class Production<T>
        {
        public:
            using Type = typename T::Expansion;
        };
    }

    template <Grammar_Rule Type>
    using Production = typename Detail::Production<typename Type::Production>::Type;
}