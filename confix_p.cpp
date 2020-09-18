#include <iostream>
#include <string>
#include <boost/spirit.hpp>
using namespace std;
using namespace boost::spirit;

struct RegExpLiteral : grammar<RegExpLiteral>
{
    struct Bracket : closure<Bracket,char> { member1 br; };
  
    template<typename ScannerT>
      struct definition
      {
          rule<ScannerT> top;
          rule<ScannerT,Bracket::context_t> expr;
          definition( const RegExpLiteral& ) {
              using phoenix::arg1;
              top  = expr;
              expr = confix_p( '/', *anychar_p, '/' );
          }
          const rule<ScannerT>& start() const  { return top; }
      };
};

int main()
{
    RegExpLiteral parser;

    string line;
    while( cout<<"# ", getline(cin, line) )
    {
        parse_info<string::const_iterator> info =
            parse( line.begin(), line.end(), parser );
        cout << (info.full ? "parsed." : "failed.") << endl;
    } 
    return 0;
}
