#include <iostream>
#include <string>
#include <boost/spirit.hpp>
using namespace std;
using namespace boost::spirit;

void display( int x ) { cout << "you typed: " << x << endl; }

struct IntDisplay : grammar<IntDisplay>
{
    template<typename ScannerT>
      struct definition
      {
          typedef rule<ScannerT> rule_t;
          rule_t r;
          definition( const IntDisplay& )
          {
              r = int_p[&display] % ',';
          }
          const rule_t& start() const { return r; }
      };
};

int main()
{
    IntDisplay parser;

    string line;
    while( cout<<"# ", getline(cin, line) )
    {
        parse_info<string::const_iterator> info =
            parse( line.begin(), line.end(), no_actions_d[parser], space_p );
        cout << (info.full ? "OK" : "fail") << endl;
    }   
    return 0;
}
