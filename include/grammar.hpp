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
}
