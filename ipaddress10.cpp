#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/spirit.hpp>
#include <boost/tokenizer.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/range/algorithm/for_each.hpp>

using namespace boost::multi_index;

using namespace std;
using namespace boost::spirit;

std::vector<string> v;

static int counter;
std::multimap<int, std::string> m_IPAddr;
std::multimap<int, std::string> m_timestamp; 

struct session {
  int linenumber;
  std::string source_ipaddr;
  std::string dest_ipaddr;
  std::string timestamp;
  
  session(int linenumber, const std::string& source_ipaddr, std::string& dest_ipaddr, std::string& timestamp)
    : linenumber(linenumber), source_ipaddr(source_ipaddr), dest_ipaddr(dest_ipaddr), timestamp(timestamp) {}

  void print() const
  { std::cout << linenumber << "," << source_ipaddr << "," << dest_ipaddr << "," << timestamp << std::endl; }
};

struct linenumber {};
struct source_ipaddr {};
struct dest_ipaddr {};
struct timestamp {};
struct seq {}; // 挿入順のタグ

typedef multi_index_container<
  session,
    indexed_by<
      ordered_unique<tag<linenumber>,   member<session, int, &session::linenumber> >,
      ordered_unique<tag<source_ipaddr>, member<session, std::string, &session::source_ipaddr> >,
      ordered_unique<tag<dest_ipaddr>, member<session, std::string, &session::dest_ipaddr> >,
      ordered_unique<tag<timestamp>, member<session, std::string, &session::timestamp> >,
      sequenced<tag<seq> >
      >
  > session_data;

void disp(int x) { std::cout << x << ' '; }

static session_data dict;

struct AddrParse : grammar<AddrParse>
{
    struct Action_ipaddr
    {
        template<typename Ite>
          void operator()( Ite i1, Ite i2 ) const
            {
	      cout << "IPaddr - line:" << counter << endl; 
	      cout << "文字数：" << i2 - i1 << endl
                   << "　内容：" << string(i1,i2) << endl;
	      cout << endl;

	      m_IPAddr.insert(std::make_pair(counter, string(i1,i2)));
	    }
    };

    struct Action_timestamp
    {
        template<typename Ite>
          void operator()( Ite i1, Ite i2 ) const
            {
	      cout << "timestamp - line:" << counter << endl;
	      cout << "文字数：" << i2 - i1 << endl
                   << "　内容：" << string(i1,i2) << endl;
	      cout << endl;

	      m_timestamp.insert(std::make_pair(counter, string(i1,i2)));
	    }
    };

    template<typename ScannerT>
      struct definition
      {
          typedef rule<ScannerT> rule_t;
          rule_t ipaddr;
	  rule_t timestamp;
	  rule_t r;
          definition( const AddrParse& self )
          {
	    // r = 'a' >> (*ch_p('b'))[MyAction()] >> 'c';
	    ipaddr = (int_p >> '.' >> int_p >> '.' >> int_p >> '.' >> int_p)[Action_ipaddr()]; 
	    timestamp = (int_p >> '-' >> int_p >> '-' >> int_p >> 'T' >> int_p >> ':' >> int_p >> ':' >> int_p >> 'Z')[Action_timestamp()]; 
	    r = timestamp | ipaddr;
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
    
    // int counter = 0;

    const auto cells = parse_csv(argv[1]);
    AddrParse parser;

    for (const auto& rows : cells) {
    
      for (const auto& cell : rows) {
	parse_info<string::const_iterator> info =
	  parse( cell.begin(), cell.end(), parser );

	if(info.full) {
	  // cout << "line:" << counter << " " << cell << endl;
	  // push_vector(cell);
	}
      }
      counter++;
    }

    ifs.close();
    
    for (auto& x:m_IPAddr) {
          for (auto& y:m_IPAddr) {
	    for (auto& z:m_timestamp) {
	    
	    if(x.first == y.first && y.first == z.first && x.second != y.second)
	      {
		// std::cout << x.first << "," << x.second << "," << y.second << std::endl;
		dict.insert(session(x.first, x.second, y.second, z.second));
	      }
	    }
	  }
    }

    for (auto & it: dict.get<0>()) {
      std::cout << it.linenumber << "," << it.timestamp << "," << it.source_ipaddr << "," << it.dest_ipaddr << std::endl;
    }
    
    return 0;
}
