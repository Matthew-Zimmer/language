#pragma once
#include <vector>

#include "alphabets.hpp"

namespace Slate::Language
{
    template <typename Starting_Rule>
    class Lexer
    {
        using Terminals = Terminal_Alphabet<Starting_Rule>;
        std::vector<Terminals> data;
    public:
        template <typename Type>
        std::vector<Terminals> tokenize(Type&& input_stream)
        {
            data.clear();
            std::string s;
            auto f = [&]()
            {
                Meta::apply<Meta::Convert<Terminals>>([&]<typename T>()
                { 
                    if (T::is_valid(s)) 
                    { 
                        s.clear(); 
                        data.emplace_back(T{});
                        return true; 
                    } 
                    else 
                        return false; 
                });
            };
            for (auto iter = input_stream.begin(); iter != input_stream.end(); ++iter)
            {  
                if (*iter == '.')
                    f();
                if (*iter != '\0' && *iter != '\t' && *iter != ' ')
                    s.push_back(*iter);
                if (*iter == ' ' || *iter == '.')
                    f();
            }
            if (s.size())
                f();
            data.emplace_back(Language::Terminals::${});
            return data;
        }
    };
}