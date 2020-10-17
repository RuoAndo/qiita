#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <boost/spirit.hpp>
#include <boost/tokenizer.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/range/algorithm/for_each.hpp>

using namespace boost::multi_index;

struct session {
  int linenumber;
  std::string source_ipaddr;
  std::string dest_ipaddr;

  session(int linenumber, const std::string& source_ipaddr, std::string& dest_ipaddr)
    : linenumber(linenumber), source_ipaddr(source_ipaddr), dest_ipaddr(dest_ipaddr) {}

  void print() const
  { std::cout << linenumber << "," << source_ipaddr << "," << dest_ipaddr << std::endl; }
};

struct linenumber {};
struct source_ipaddr {};
struct dest_ipaddr {};
struct seq {}; // 挿入順のタグ

typedef multi_index_container<
  session,
    indexed_by<
      ordered_unique<tag<linenumber>,   member<session, int, &session::linenumber> >,
      ordered_unique<tag<source_ipaddr>, member<session, std::string, &session::source_ipaddr> >,
      ordered_unique<tag<dest_ipaddr>, member<session, std::string, &session::dest_ipaddr> >,
      sequenced<tag<seq> >
      >
  > dictionary;

void disp(int x) { std::cout << x << ' '; }

static dictionary dict;

using namespace std;
using namespace boost::spirit;

static int line_counter = 0;
static int counter = 0;
std::multimap<int, std::string> m; 

struct AddrParse : grammar<AddrParse>
{
    struct MyAction
    {
        template<typename Ite>
          void operator()( Ite i1, Ite i2 ) const
            {
	      cout << "文字数：" << i2 - i1 << endl
                   << "　内容：" << string(i1,i2) << "," << line_counter << endl;	      
	      m.insert(std::make_pair(line_counter, string(i1,i2)));
	      // dict.insert(session(line_counter, string(i1,i2), int(i2-i1)));
	    }
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

    typedef dictionary::index<seq>::type List;
    
    // int counter = 0;

    const auto cells = parse_csv(argv[1]);
    AddrParse parser;

    for (const auto& rows : cells) {
    
      for (const auto& cell : rows) {
	parse_info<string::const_iterator> info =
	  parse( cell.begin(), cell.end(), parser );

	if(info.full) {
	  cout << "line:" << line_counter << " " << cell << ":" << counter << endl;
	  // push_vector(cell);
	}
      }
      line_counter++;
      // counter++;
    }
       
    ifs.close();

    cout << "Displaying elements..." << endl;

    for (auto& x:m) {
      // std::cout << x.first << " => " << x.second << std::endl;

          for (auto& y:m) {
	    if(x.first == y.first && x.second != y.second)
	      {
		// std::cout << x.first << "," << x.second << "," << y.second << std::endl;
		dict.insert(session(x.first, x.second,  y.second));
	      }
	  }
    }

    for (auto & it: dict.get<0>()) {
      std::cout << it.linenumber << "," << it.source_ipaddr << "," << it.dest_ipaddr << std::endl;
    }
    
    return 0;
}
