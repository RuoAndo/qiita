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
            {
	      if(i2 - i1 > 0)
		{
		  cout << "# of characters: " << i2 - i1 << endl
		       << "string: " << string(i1,i2) << endl;
		}
	    }
    };

    template<typename ScannerT>
      struct definition
      {
          typedef rule<ScannerT> rule_t;
	  rule_t r; rule_t r_list;
          definition( const Abc& self )
          {
	    r = (+alnum_p >> '_' >> alnum_p | +alnum_p >> '*' | *alnum_p )[MyAction()];
	    r_list = r % +space_p;
          }
          const rule_t& start() const { return r_list; }
      };
};

#include <typeinfo>
int main(int argc, char* argv[]){
    using namespace std;

    if(argc != 2){
        cerr << "wrong # of args." << endl;
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

       if( parse( tmp.begin(), tmp.end(), Abc() ).full );
       cout << endl;
       
       counter++;
    }

    ifs.close();
    return 0;
}

