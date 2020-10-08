#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/spirit.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost::spirit;

std::vector<string> v;

struct AddrParse : grammar<AddrParse>
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
          rule_t r;
          definition( const AddrParse& self )
          {
	    // r = 'a' >> (*ch_p('b'))[MyAction()] >> 'c';
	    r = (int_p >> '.' >> int_p >> '.' >> int_p >> '.' >> int_p)[MyAction()]; // >> +( '*' >> int_p );
          }
          const rule_t& start() const { return r; }
      };
};

std::vector < std::vector< std::string > > parse_csv(const char* filepath)
{
    std::vector< std::vector< std::string > > cells;
    std::string line;
    std::ifstream ifs(filepath);

    while (std::getline(ifs, line)) {

        std::vector< std::string > data;

        boost::tokenizer< boost::escaped_list_separator< char > > tokens(line);
        for (const std::string& token : tokens) {
            data.push_back(token);
        }

        cells.push_back(data);
    }

    return cells;
}

#include <typeinfo>
int main(int argc, char* argv[]){
    using namespace std;

    if(argc != 2){
        cerr << "引数の数が間違っています．" << endl;
        cerr << "./spirit_file_read [INPUT_FILE_NAME]" << endl;
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

    const auto cells = parse_csv(argv[1]);
    AddrParse parser;

    for (const auto& rows : cells) {
    
      for (const auto& cell : rows) {
	parse_info<string::const_iterator> info =
	  parse( cell.begin(), cell.end(), parser );

	if(info.full) {
	  cout << "line:" << counter << " " << cell << endl;
	  // push_vector(cell);
	}
      }

      counter++;
    }
       
    ifs.close();
    return 0;
}
