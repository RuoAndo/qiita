#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_container.hpp>

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

using namespace boost::spirit;
using namespace boost::spirit::ascii;

enum tokenids
  {
    IDANY = boost::spirit::lex::min_token_id + 1,
  };

inline std::string read_from_file( const char* infile )
{
  std::ifstream instream( infile );
  if( !instream.is_open() )
    {
      std::cerr << "Could not open file: \"" << infile << "\"" << std::endl;
      exit( -1 );
    }
  instream.unsetf( std::ios::skipws );
  return( std::string(
		      std::istreambuf_iterator< char >( instream.rdbuf() ),
		      std::istreambuf_iterator< char >()
		      ) );
}

template <typename Lexer>
struct word_count_tokens : lex::lexer<Lexer>
{
  word_count_tokens()
  {
    // define patterns (lexer macros) to be used during token definition 
    // below
        this->self.add_pattern
	  ("WORD", "[^ \t\n]+")
	  ;

        // define tokens and associate them with the lexer
        word = "{WORD}";    // reference the pattern 'WORD' as defined above

        // this lexer will recognize 3 token types: words, newlines, and 
        // everything else
        this->self.add
	  (word)          // no token id is needed here
	  ('\n')          // characters are usable as tokens as well
	  (".", IDANY)    // string literals will not be escaped by the library
	  ;
  }

  // the token 'word' exposes the matched string as its parser attribute
  lex::token_def<std::string> word;
};

template <typename Iterator>
struct word_count_grammar : qi::grammar<Iterator>
{
  template <typename TokenDef>
  word_count_grammar(TokenDef const& tok)
    : word_count_grammar::base_type(start)
    , c(0), w(0), l(0)
  {
    using boost::phoenix::ref;
    using boost::phoenix::size;

    start =  *(   tok.word          [++ref(w), ref(c) += size(_1)]
                  |   lit('\n')         [++ref(c), ++ref(l)]
                  |   qi::token(IDANY)  [++ref(c)]
                  )
      ;
  }

  std::size_t c, w, l;
  qi::rule<Iterator> start;
};

int main(int argc, char* argv[])
{
  typedef lex::lexertl::token<
    char const*, boost::mpl::vector<std::string>
    > token_type;

  typedef lex::lexertl::lexer<token_type> lexer_type;

  typedef word_count_tokens<lexer_type>::iterator_type iterator_type;

  // now we use the types defined above to create the lexer and grammar
  // object instances needed to invoke the parsing process
  word_count_tokens<lexer_type> word_count;          // Our lexer
  word_count_grammar<iterator_type> g (word_count);  // Our parser 

  // read in the file int memory
  std::string str (read_from_file(1 == argc ? "word_count.input" : argv[1]));
  // std::string str = __fromfpl(1 == argc ? "word_count.input" : argv[1]);
  char const* first = str.c_str();
  char const* last = &first[str.size()];

  bool r = lex::tokenize_and_parse(first, last, word_count, g);

  if (r) {
    std::cout << "lines: " << g.l << ", words: " << g.w
	      << ", characters: " << g.c << "\n";
  }
  else {
    std::string rest(first, last);
    std::cerr << "Parsing failed\n" << "stopped at: \""
	      << rest << "\"\n";
  }
  return 0;
}
