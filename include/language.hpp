#include <iostream>
#include <stack>
#include <variant>
#include <vector>
#include <unordered_map>

#include <meta/meta.hpp>

template<class... Ts> 
class Overloaded : public Ts... 
{
public:
    using Ts::operator()...; 
};
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

namespace Slate::Language
{
    template <typename Type>
    concept Language_Grammar = requires()
    {
        typename Type::Production;
    };

    enum class Parsing_State
    {
        accept,
        reject,
        parsing
    };

    class Terminate 
    {
    public:
        void Print()
        {
            std::cout << "$" << std::endl;
        }
        static bool Is_Valid(std::string const&)
        {
            return false;
        }
    };

    namespace Detail
    {
        using namespace Slate::Meta;

        template <std::size_t N>
        class Fixed_String_I
        {
        public:
            char data[N+1];
            consteval Fixed_String_I(const char (&foo)[N+1])
            { 
                std::copy_n(foo, N+1, data); 
            }

            bool operator==(std::string const& s) const
            {
                return s.size() == N && std::equal(data, data + N, s.begin());
            }

            friend std::ostream& operator<<(std::ostream& stream, Fixed_String_I s)
            {
                std::cout << s.data;
                return stream;
            }
        };
        
        template <std::size_t N>
        Fixed_String_I(const char (&str)[N]) -> Fixed_String_I<N - 1>;

        class Dot_Marker
        {};

        template <typename Type>
        class Start_I
        {
        public:
            using Production = Wrap<Wrap<Type>>;
            void Print()
            {
                std::cout << "START_STATE" << std::endl;
            }
        };

        template <typename Rule>
        class Dot_Operator_I
        {
        public:
            template <typename T>
            class Dot_Expansion
            {
            public:
                using Type = Wrap<Wrap<Rule, Wrap<>, Dot_Marker, T>>;
            };
        };
    }

    template <Language_Grammar Type>
    using Production = typename Type::Production;

    template <Language_Grammar Type>
    using Dot = Meta::For_Each<Production<Type>, Detail::Dot_Operator_I<Type>::template Dot_Expansion>;  


    namespace Detail
    {
        template <typename>
        class Composition_I
        {};

        template <typename Rule, typename ... Pre, Slate::Language::Language_Grammar F, typename ... Post>
        class Composition_I<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>
        {
            using Original_Type = Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>;
        public:
            using Type = Join<Original_Type, For_Each<Dot<F>, Composition_I>>;
        };

        template <typename Rule, typename ... Pre, typename F, typename ... Post>
        class Composition_I<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>
        {
            using Original_Type = Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>;
        public:
            using Type = Original_Type;
        };

        template <typename Rule, typename ... Pre>
        class Composition_I<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<>>>
        {
            using Original_Type = Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<>>>;
        public:
            using Type = Original_Type;
        };
    }

    template <typename Type>
    class Parse_Tree
    {};

    template <Detail::Fixed_String_I Str>
    class Keyword 
    {
    public:
        static bool Is_Valid(std::string const& s)
        {
            return Str == s;
        }
        void Print()
        {
            std::cout << Str << std::endl;
        }
    };

    class Slash 
    {};

    template <typename Type, typename ... Types>
    class Grammar
    {
    public:
        using Production = Meta::Split<Meta::Wrap<Types...>, Slash>;
    };

    template <typename Type>
    using Composition = Meta::For_Each<Type, Detail::Composition_I>;

    namespace Detail
    {
        template <typename Type1, typename Type2, typename=void>
        class Valid_Next_I
        {
        public:
            using Type = Wrap<>;
        };

        template <typename Type1, typename Type2>
        class Valid_Next_I<Type1, Type2, std::void_t<decltype(Type1::template Next_Imp<Type2>())>>
        {
        public:
            using Type = decltype(Type1::template Next_Imp<Type2>());
        };
    }

    template <typename Type1, typename Type2>
    using Valid_Next = typename Detail::Valid_Next_I<Type1, Type2>::Type;

    template <typename Type1, typename Type2>
    constexpr bool Has_Valid_Next = !std::is_same_v<Valid_Next<Type1, Type2>, Meta::Wrap<>>;

    namespace Detail
    {

        template <typename Type1, typename Type2, typename, typename ... Args>
        class Has_Action_I : public std::false_type
        {};

        template <typename Type1, typename Type2, typename ... Args>
        class Has_Action_I<Type1, Type2, std::void_t<decltype(Type1::template Parse_Action<Type2>(*static_cast<Args*>(nullptr)...))>, Args...> : public std::true_type
        {};
    }

    template <typename Type1, typename Type2, typename ... Args>
    constexpr bool Has_Action = Detail::Has_Action_I<Type1, Type2, void, Args...>::value;

    namespace Detail
    {
        template <typename Type>
        class Is_Start_State_I : public std::false_type
        {};

        template <typename Type>
        class Is_Start_State_I<Start_I<Type>> : public std::true_type
        {};
    }

    template <typename Type>
    constexpr bool Is_Start_State = Detail::Is_Start_State_I<Type>::value;

    namespace Detail
    {
        template <typename Type, typename State>
        class Action_I
        {
        public:
            template <typename T, typename Iter, typename State_Stack, typename Parse_Stack> 
            static Parsing_State Parse_Action(Iter& i, State_Stack& s, Parse_Stack& p) requires(std::is_same_v<T, Type>)//shift
            {
                std::cout << "SHIFT: ";
                T{}.Print();
                p.push_back(*i++);
                s.push_back(Valid_Next<State, T>{});
                return Parsing_State::parsing;
            }
        };

        template <Slate::Language::Language_Grammar Type, typename State>
        class Action_I<Type, State>
        {
        public:
            template <typename T, typename Iter, typename State_Stack, typename Parse_Stack> 
            static Parsing_State Parse_Action(Iter& i, State_Stack& s, Parse_Stack& p) requires(std::is_same_v<T, Type>)//goto
            {
                std::cout << "GOTO: ";
                T{}.Print();
                s.push_back(Valid_Next<State, T>{});
                return Parsing_State::parsing;
            }
        };
    }

    namespace Detail
    {
        template <typename>
        class State_I {};

        template <typename Rule, typename ... Pre, typename F, typename ... Post, typename ... Rest>
        class State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>, Rest...>> : public State_I<Wrap<Rest...>>, public Action_I<F, State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>, Rest...>>>
        {
            using This = State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>, Rest...>>;
        public:
            template <typename T> requires(std::is_same_v<T, F>)
            static State_I<Composition<Wrap<Wrap<Rule, Wrap<Pre..., F>, Dot_Marker, Wrap<Post...>>>>> Next_Imp();
            using State_I<Wrap<Rest...>>::Next_Imp;
            
            using Action_I<F, This>::Parse_Action;
            using State_I<Wrap<Rest...>>::Parse_Action;

            template <typename T>
            using Next = decltype(This::Next_Imp<T>());

            void Print()
            {
                Rule{}.Print();
            }
        };

        template <typename Rule, typename ... Pre, typename F, typename ... Post>
        class State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>> : public Action_I<F, State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>>>
        {
            using This = State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<F, Post...>>>>;
        public:
            template <typename T> requires(std::is_same_v<T, F>)
            static State_I<Composition<Wrap<Wrap<Rule, Wrap<Pre..., F>, Dot_Marker, Wrap<Post...>>>>> Next_Imp();

            using Action_I<F, This>::Parse_Action;

            template <typename T>
            using Next = decltype(This::Next_Imp<T>());

            void Print()
            {
                Rule{}.Print();
            }
        };

        template <typename Rule, typename ... Pre>
        class State_I<Wrap<Wrap<Rule, Wrap<Pre...>, Dot_Marker, Wrap<>>>>
        {
            template <typename Iter, typename State_Stack, typename Parse_Stack>
            class Reduce_Visitor
            {
                Iter& i;
                State_Stack& s;
                Parse_Stack& p;
            public:
                Reduce_Visitor(Iter& i, State_Stack& s, Parse_Stack& p) : i{ i }, s{ s }, p{ p } 
                {}
                template <typename T>
                Parsing_State operator()(T)
                {
                    if constexpr(Slate::Language::Has_Action<T, Rule, Iter, State_Stack, Parse_Stack>)
                        return T::template Parse_Action<Rule>(i, s, p);
                    else
                        return Parsing_State::reject;
                }
            };
        public:
            template <typename T, typename Iter, typename State_Stack, typename Parse_Stack> 
            static Parsing_State Parse_Action(Iter& iter, State_Stack& s, Parse_Stack& p) requires(!Language_Grammar<T> && !Is_Start_State<Rule>)//reduce
            {
                std::cout << "Reduce: ";
                Rule{}.Print();
                for (std::size_t i{ 0 }; i < sizeof...(Pre);i++)
                {
                    p.pop_back();
                    s.pop_back();
                }
                p.push_back(Rule{});
                return std::visit(Reduce_Visitor<Iter, State_Stack, Parse_Stack>{ iter, s, p }, s.back());
            }

            template <typename T, typename Iter, typename State_Stack, typename Parse_Stack> 
            static Parsing_State Parse_Action(Iter& iter, State_Stack& s, Parse_Stack& p) requires(std::is_same_v<T, Terminate> && Is_Start_State<Rule>)//accept
            {
                return Parsing_State::accept;
            }

            void Print()
            {
                Rule{}.Print();
            }
        };
    }

    template <typename Grammar>
    using State = Detail::State_I<Composition<Dot<Grammar>>>;

    namespace Detail
    {
        template <typename Alphabet, typename Start_State>
        class States_I
        {};

        template <typename ... Alphabet_Types, typename Start_State>
        class States_I<Wrap<Alphabet_Types...>, Start_State>
        {
            template <typename T, typename=void>
            class Use_If_Not_Me
            {
            public:
                using Type = typename States_I<Wrap<Alphabet_Types...>, T>::Type;
            };

            template <typename T>
            class Use_If_Not_Me<T, std::enable_if_t<std::is_same_v<T, Start_State>>>
            {
            public:
                using Type = Wrap<>; 
            };
        public:
            using Type = Join<
                Start_State,
                Valid_Next<Start_State, Alphabet_Types>..., 
                typename Use_If_Not_Me<Valid_Next<Start_State, Alphabet_Types>>::Type...>;
        };

        template <typename...>
        class Starting_Grammar_I 
        {};

        template <typename First, typename... Rest>
        class Starting_Grammar_I<First, Rest...>
        {
        public:
            using Type = Start_I<First>;
        };
    }

    template <typename ... Grammars>
    using Starting_Grammar = typename Detail::Starting_Grammar_I<Grammars...>::Type;

    template <typename ... Grammars>
    using Start_State = State<Starting_Grammar<Grammars...>>;

    template <typename ... Grammars>
    using State_Alphabet = Meta::Convert<Meta::Unique<typename Detail::States_I<Meta::Wrap<Grammars...>, Start_State<Grammars...>>::Type>, std::variant>;

    template <typename ... Grammars>
    using Token_Alphabet = std::variant<Starting_Grammar<Grammars...>, Grammars..., Terminate>;

    template <typename Iter, typename State_Stack, typename Parse_Stack>
    class State_Visitor
    {
        Iter& i;
        State_Stack& s;
        Parse_Stack& p;
    public:
        State_Visitor(Iter& i, State_Stack& s, Parse_Stack& p) : i{ i }, s{ s }, p{ p } 
        {}
        template <typename Type1, typename Type2>
        Parsing_State operator()(Type1, Type2)
        {
            if constexpr (Has_Action<Type1, Type2, Iter, State_Stack, Parse_Stack>)
                return Type1::template Parse_Action<Type2>(i, s, p);
            else
            {
                std::cout << "Type1: ";
                Type1{}.Print();
                std::cout << "Type2: ";
                Type2{}.Print();
                return Parsing_State::reject;
            }
        }
    };

    template <typename ... Grammars>
    class Lexer
    {
        std::vector<Token_Alphabet<Grammars...>> data;
    public:
        template <typename Type>
        auto Tokenize(Type&& input_stream)
        {
            data.clear();
            std::string s;
            for (auto iter = input_stream.begin(); iter != input_stream.end(); ++iter)
            {
                if (*iter != '\0' && *iter != '\t' && *iter != ' ')
                    s.push_back(*iter);
                ([&]()
                {
                    if constexpr(!Language_Grammar<Grammars>)
                        if (Grammars::Is_Valid(s))
                        {
                            data.emplace_back(Grammars{});
                            s.clear();
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    else
                        return false;
                }() || ...);
            }
            data.emplace_back(Terminate{});


            for(auto& x : data)
                std::visit(Overloaded{
                    [](auto x) { if constexpr(!Language_Grammar<decltype(x)>)x.Print(); },
                }, x);
            return data;
        }
    };

    template <typename Type>
    class LR_Parser_Base 
    {
    public:
        template <typename Token_Stream>
        auto Parse(Token_Stream const& tokens)
        {
            auto iter = tokens.begin();
            Meta::Cast<Type>(*this).state_stack.emplace_back(Meta::Cast<Type>(*this).Initial_State());
            State_Visitor visitor{ iter, Meta::Cast<Type>(*this).state_stack, Meta::Cast<Type>(*this).parse_stack };
            while (true)
                if (auto state = std::visit(visitor, Meta::Cast<Type>(*this).state_stack.back(), *iter); state == Parsing_State::accept)
                {
                    Meta::Cast<Type>(*this).add_parsed_entry(Meta::Cast<Type>(*this).parse_stack.back());
                    break;
                }
                else if (state == Parsing_State::reject)
                {
                    throw std::runtime_error{ "Invalid code" };
                }
            return Meta::Cast<Type>(*this).data;
        }
    };

    template <typename ... Grammars>
    class LR0 : public LR_Parser_Base<LR0<Grammars...>>
    {
        using States = State_Alphabet<Grammars...>;
        using Augmented_State = Start_State<Grammars...>;
        using Tokens = Token_Alphabet<Grammars...>;
        using Principle_Token = Meta::Unwrap<Meta::Wrap<Grammars...>>;
    public:
        std::vector<States> state_stack;
        std::vector<Tokens> parse_stack;
        std::vector<Principle_Token> data;

        auto Initial_State()
        {
            return Augmented_State{};
        }

        void add_parsed_entry(Tokens const& token)
        {
            std::visit(Overloaded{
                [&](auto const&){},
                [&](Principle_Token const& t){ data.push_back(t); }
            }, token);
        }
    };

    template <typename Type>
    class SLR
    {};

    template <typename Type>
    class CLR
    {};

    template <typename Type>
    class LALR
    {};
}