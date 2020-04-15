#pragma once
#include "concepts.hpp"

namespace Slate::Language
{
    template <Grammar_Rule Type>
    using Production = Meta::Wrap<typename Type::Production>;
}