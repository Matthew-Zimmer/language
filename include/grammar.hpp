#pragma once
#include "concepts.hpp"


namespace Slate::Language
{
    namespace Detail
    {
        template <std::size_t N>
        class Fixed_String
        {
        public:
            char data[N+1];
            consteval Fixed_String(const char (&foo)[N+1])
            { 
                std::copy_n(foo, N+1, data); 
            }

            bool operator==(std::string const& s) const
            {
                return s.size() == N && std::equal(data, data + N, s.begin());
            }

            // friend std::ostream& operator<<(std::ostream& stream, Fixed_String s)
            // {
            //     std::cout << s.data;
            //     return stream;
            // }
        };
        
        template <std::size_t N>
        Fixed_String(const char (&str)[N]) -> Fixed_String<N - 1>;
    }

    namespace Terminals 
    {
        class $
        {
        public:
            static bool is_valid(std::string const&)
            {
                return false;
            }
        };

        template <Detail::Fixed_String Str>
        class Keyword
        {
        public:
            static bool is_valid(std::string const& s)
            {
                return Str == s;
            }
            static std::string str() 
            {
                return Str.data;
            }
        };
    }

    namespace Rules 
    {
        template <typename T>
        class Rule
        {
        public:
            using Production = T;
        };

        template <typename Type>
        using Start = Rule<Type>;
    }

    namespace Expansions
    {
        template <typename Type>
        class Star
        {
        public:
            
        };

        template <typename ... Types>
        class Join;

        template <typename ... Types>
        class Or
        {
            template <typename T, typename V>
            class Builder
            {
            public:
                using Type = Meta::Append<V, Meta::Wrap<Meta::Wrap<T>>>;
            };
            template <typename ... Join_Types, typename Value>
            class Builder<Join<Join_Types...>, Value>
            {
            public:
                using Type = Meta::Join<Value, typename Join<Join_Types...>::Expansion>;
            };
            template <typename ... Or_Types, typename Value>
            class Builder<Or<Or_Types...>, Value>
            {
            public:
                    using Type = Meta::Join<Value, typename Or<Or_Types...>::Expansion>;
            };
        public:
            using Expansion = Meta::Left_Fold<Meta::Wrap<Types...>, Builder>;
        };

        template <typename ... Types>
        class Join
        {
            template <typename T, typename V>
            class Builder
            {
            public:
                using Type = Meta::For_Each<V, Meta::Reverse_Adapter<Meta::Append, T>::template Function>;
            };
            template <typename ... Join_Types, typename Value>
            class Builder<Join<Join_Types...>, Value>
            {
                template <typename U, typename V>
                class Expand
                {
                public:
                    using Type = Meta::For_Each<U, Meta::Adapter<Meta::Append, V>::template Function>;
                };
            public:
                using Type = Meta::Join_For_Each<Value, Meta::Adapter<Expand, typename Join<Join_Types...>::Expansion>::template Function>;
            };
            template <typename ... Or_Types, typename Value>
            class Builder<Or<Or_Types...>, Value>
            {
                template <typename U, typename V>
                class Expand
                {
                public:
                    using Type = Meta::For_Each<U, Meta::Adapter<Meta::Append, V>::template Function>;
                };
            public:
                using Type = Meta::Join_For_Each<Value, Meta::Adapter<Expand, typename Or<Or_Types...>::Expansion>::template Function>;
            };
        public:
            using Expansion = Meta::Left_Fold<Meta::Wrap<Types...>, Builder, Meta::Wrap<Meta::Wrap<>>>;
        };
    }
}