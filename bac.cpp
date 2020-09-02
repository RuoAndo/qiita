#include <iostream>
#include <string>
#include <boost/spirit.hpp>
using namespace std;
using namespace boost::spirit;

struct Watashino_Grammar : grammar<Watashino_Grammar>
{
    template<typename ScannerT>
      struct definition
      {
          typedef rule<ScannerT> rule_t;
          rule_t r;

          definition( const Watashino_Grammar& )
          {
              r = ch_p('b') >> *ch_p('a') >> ch_p('c');
          }
          const rule_t& start() const { return r; }
      };
};

#include <typeinfo>
int main()
{
    Watashino_Grammar parser;

    string line;
    while( cout<<"# ", getline(cin, line) )
    {
        parse_info<string::const_iterator> info =
            parse( line.begin(), line.end(), parser );
        cout << (info.full ? "OK" : "fail") << endl;
    }  
    return 0;
}
