#pragma once
#include "next.hpp"

namespace Slate::Language
{
    namespace Detail
    {
        template <typename>
        class State;

        template <typename T>
        class Colapse
        {
            template <typename, typename>
            class Builder
            {};
            template <typename ... Us, typename V>
            class Builder<Meta::Wrap<Us...>, V>
            {
                template <typename, typename, typename>
                class Value{};
                template <typename S, typename ... Xs, typename ... Ys>
                class Value<S, Meta::Wrap<Xs...>, Meta::Wrap<Ys...>>
                {
                public:
                    using Type = Meta::Use_If<([]{ using X = Xs; return (std::is_same_v<X, Ys> || ...); }() || ...), Meta::Unique<Meta::Join<S, V>>>;
                };
                using Combined = Meta::Join<typename Value<Us, typename Next_Token<Us>::Type, typename Next_Token<V>::Type>::Type...>;
            public:
                using Type = std::conditional_t<std::is_same_v<Combined, Meta::Wrap<>>, V, Combined>;
            };
        public:
            using Type = Meta::For_Each<T, Meta::Adapter<Builder, T>::template Function>;
        };
    }

    template <typename T>
    using Colapse = typename Detail::Colapse<T>::Type;
}