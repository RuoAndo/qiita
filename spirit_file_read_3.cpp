#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <boost/spirit.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
using namespace std;
using namespace boost::spirit;

struct Abc : grammar<Abc>
{
    struct MyAction
    {
        template<typename Ite>
          void operator()( Ite i1, Ite i2 ) const
            { cout << "文字数：" << i2 - i1 << endl
                   << "　内容：" << string(i1,i2) << endl; }
    };

    template<typename ScannerT>
      struct definition
      {
          typedef rule<ScannerT> rule_t;
	  rule_t r; rule_t r_list;
          definition( const Abc& self )
          {
	    // r = (*anychar_p)[MyAction()];
	    // r_list = r % +blank_p;
	    // r = (+alnum_p)[MyAction()] % ',';
	    r = (+alnum_p >> blank_p)[MyAction()]; //' '; // blank_p;
            // r = 'a' >> (*ch_p('b'))[MyAction()] >> 'c';
          }
          const rule_t& start() const { return r; }
      };
};

/*
int main()
{
    for( string line; cout<<"# ", getline(cin, line); )
        if( parse( line.begin(), line.end(), Abc() ).full );
    return 0;
}
*/

#include <typeinfo>
int main(int argc, char* argv[]){
    using namespace std;

    if(argc != 2){
        cerr << "引数の数が間違っています．" << endl;
        cerr << "./spirit_file_read input.txt" << endl;
        return 1;
    }

    ifstream ifs(argv[1], ios::in);
    if(!ifs){
        cerr << "Error: file not opened." << endl;
        return 1;
    }

    string tmp;
    string str;
    
    int counter = 0;

    Abc parser;
    
    while(getline(ifs, tmp)){

        cout << "line:" << counter << ":" << tmp << endl; 

        parse_info<string::const_iterator> info =
	parse( tmp.begin(), tmp.end(), Abc() );
        cout << (info.full ? "OK" : "") << endl;

	counter++;
    }

    ifs.close();
    return 0;
}

