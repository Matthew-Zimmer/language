// #include <ordeal/ordeal.hpp>

// #include "lexer.hpp"
// #include "parser.hpp"

// using namespace Slate::Language;
// using namespace Slate::Meta;
// using namespace Slate::Ordeal;

// using namespace Expansions;

// class A{};
// class B{};
// class C{};
// class D{};
// class E{};

// template <typename U, typename V>
// void test()
// {
//     static_assert(std::is_same_v<U, V>);
// }

// template <typename T>
// using P = Production<Rules::Rule<T>>;

// void f()
// {
//     using Slate::Language::Expansions::Join;
//     using Slate::Meta::Wrap;

    
    
//     // JOIN
    
//     // simple in join
//     test<P<Join<A, B>>, Wrap<Wrap<A, B>>>();

//     // join in join
//     test<P<Join<Join<A, B>>>, Wrap<Wrap<A, B>>>();

//     // join simple in join
//     test<P<Join<Join<A, B>, C>>, Wrap<Wrap<A, B, C>>>();

//     // simple join in join
//     test<P<Join<A, Join<B, C>>>, Wrap<Wrap<A, B, C>>>();
    
//     // join join in join
//     test<P<Join<Join<A, B>, Join<C, D>>>, Wrap<Wrap<A, B, C, D>>>();

//     // join join simple in join
//     test<P<Join<Join<A, B>, Join<C, D>, E>>, Wrap<Wrap<A, B, C, D, E>>>();

//     // join join join in join
//     test<P<Join<Join<A, B>, Join<C, D>, Join<E>>>, Wrap<Wrap<A, B, C, D, E>>>();

//     // simple or in join
//     test<P<Join<A, Or<B, C>>>, Wrap<Wrap<A, B>, Wrap<A, C>>>();

//     // simple or simple in join
//     test<P<Join<A, Or<B, C>, D>>, Wrap<Wrap<A, B, D>, Wrap<A, C, D>>>();

//     // or join in join
//     test<P<Join<Or<A, B>, Join<C, D>>>, Wrap<Wrap<A, C, D>, Wrap<B, C, D>>>();

//     // join or in join
//     test<P<Join<Join<A, B>, Or<C, D>>>, Wrap<Wrap<A, B, C>, Wrap<A, B, D>>>();

//     // or or in join
//     test<P<Join<Or<A, B>, Or<C, D>>>, Wrap<Wrap<A, C>, Wrap<A, D>, Wrap<B, C>, Wrap<B, D>>>();

//     // or or simple in join
//     test<P<Join<Or<A, B>, Or<C, D>, E>>, Wrap<Wrap<A, C, E>, Wrap<A, D, E>, Wrap<B, C, E>, Wrap<B, D, E>>>();

//     // simple Join<Join> in join
//     test<P<Join<Join<A, Join<B, C>>>>, Wrap<Wrap<A, B, C>>>();

//     // Join<Join> simple in join
//     test<P<Join<Join<Join<A, B>, C>>>, Wrap<Wrap<A, B, C>>>();

//     // Join<Join> simple in join
//     test<P<Join<A, Join<Join<B, C>, D>>>, Wrap<Wrap<A, B, C, D>>>();

//     // OR

//     // simple in or
//     test<P<Or<A, B>>, Wrap<Wrap<A>, Wrap<B>>>();

//     // join in Or
//     test<P<Or<Join<A, B>>>, Wrap<Wrap<A, B>>>();

//     // join simple in or
//     test<P<Or<Join<A, B>, C>>, Wrap<Wrap<A, B>, Wrap<C>>>();

//     // simple join in or
//     test<P<Or<A, Join<B, C>>>, Wrap<Wrap<A>, Wrap<B, C>>>();

//     // or in or
//     test<P<Or<Or<A, B>>>, Wrap<Wrap<A>, Wrap<B>>>();

//     // or or in or
//     test<P<Or<Or<A, B>, Or<C, D>>>, Wrap<Wrap<A>, Wrap<B>, Wrap<C>, Wrap<D>>>();

//     // or<simple, or> in or
//     test<P<Or<Or<A, Or<B, C>>>>, Wrap<Wrap<A>, Wrap<B>, Wrap<C>>>();

//     // TOGETHER

//     // or<simple join<simple, or>> simple
//     test<P<Or<A, Join<B, Or<C, D>>, E>>, Wrap<Wrap<A>, Wrap<B, C>, Wrap<B, D>, Wrap<E>>>(); 

//     // join<simple or<simple join> simple>
//     test<P<Join<A, Or<B, Join<C, D>>, E>>, Wrap<Wrap<A, B, E>, Wrap<A, C, D, E>>>(); 
// }