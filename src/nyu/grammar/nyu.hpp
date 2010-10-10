#ifndef NYU_GRAMMAR_GRAMMAR_HPP
#define NYU_GRAMMAR_GRAMMAR_HPP

#include <chilon/parser/source_code_stream.hpp>
#include <chilon/parser/sequence.hpp>
#include <chilon/parser/ascii.hpp>
#include <chilon/parser/joined.hpp>
#include <chilon/parser/choice.hpp>
#include <chilon/parser/until.hpp>
#include <chilon/parser/any_char.hpp>
#include <chilon/parser/not_char.hpp>
#include <chilon/parser/char.hpp>
#include <chilon/parser/char_range.hpp>
#include <chilon/parser/lexeme.hpp>
#include <chilon/parser/not.hpp>
#include <chilon/parser/optional.hpp>
#include <chilon/parser/simple_node.hpp>
#include <chilon/parser/tree_joined.hpp>
#include <chilon/parser/tree_many.hpp>

namespace nyu { namespace grammar {

struct NyuRule {
    enum class Status {
        UNKNOWN,
        NORMAL, // can be a node
        HASHED,
        NODE   // set while processing dependencies of node
    };

    NyuRule() : status_(Status::UNKNOWN) {}
    Status status_;
};

namespace nyu {

using namespace chilon::parser;
using namespace chilon::parser::ascii;

typedef choice<
    sequence<
        char_<'/', '/'>,
        until<char_<'\n'>, any_char>
    >,
    whitespace
> Spacing;

typedef char_<'.'> AnyCharacter;

struct CharacterRange : simple_node<CharacterRange, lexeme<
    char_<'['>,
    many<
        choice<
            lexeme<
                store<char_<'\\'>>,
                char_from<'\\',']',s,S,n,N,t,T,w,W> >,
            sequence< not_char<']'>, char_<'-'>, not_char<']'> >,
            not_char<']'> > >,
    char_<']'>
> > {};

typedef lexeme<
    store<char_<'\\'>>,
    char_from<s,S,n,N,t,T,w,W,'.','"','\'','&','!','+','*','\\'>> Escape;

struct String : simple_node<String, choice<
    lexeme< char_<'"'>,
        many_range< choice<Escape, not_char<'"'> > >,
    char_<'"'> >,
    lexeme< char_<'\''>,
        many_range< choice<Escape, not_char<'\''> > >,
    char_<'\''> >
> > {};

typedef lexeme<
    choice<char_<'_'>, char_range<a,z, A,Z> >,
    many< choice<
        char_range<a,z, A,Z, '0','9'>,
        char_<'_'>
    > > > Identifier;

typedef joined_plus<char_<':', ':'>, Identifier> ScopedIdentifier;

struct Expression;

typedef choice<
    String, CharacterRange, Escape, AnyCharacter,
    sequence< ScopedIdentifier, not_< char_<'<'> > >,
    sequence< char_<'('>, node<Expression>, char_<')'> >
> Primary;

typedef choice<
    char_from<'+', '?', '*'>,
    char_<'^', '+'>,
    char_<'^', '*'>,
    char_<'|', '+'>,
    char_<'|', '?'> > Suffixes;

struct Suffix
  : simple_node<Suffix, Primary, tree_optional<Suffixes>> {};

typedef choice<
    char_<'&', '!'>, char_<'#', '+'>, char_from<'&', '!', '#'> > Prefixes;

struct Prefix
  : simple_node<Prefix, tree_optional<Prefixes>, Suffix> {};

struct Join : simple_node<Join,
    Prefix,
    choice<
        char_<'^', '%'>,
        char_<'%', '+'>,
        char_<'%'>,
        char_<'|', '%'>,
        char_<'|', '^', '%'> >,
    Prefix> {};

struct Joined : simple_node<Joined,
    tree_joined<char_<'^'>, choice<Join, Prefix> > > {};

struct Sequence : simple_node<Sequence, tree_many<Joined> > {};

struct OrderedChoice : simple_node<OrderedChoice,
    tree_joined<char_<'/'>, Sequence> > {};

struct Expression : simple_node<Expression, OrderedChoice> {};

struct Rule : NyuRule, simple_node<Rule,
    key<Identifier>,
    char_<'<'>, optional<ScopedIdentifier>, char_from<'=', '-'>,
    Expression > {};

typedef many<Rule> Grammar;

} } }
#endif
