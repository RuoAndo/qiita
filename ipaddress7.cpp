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
  int bytes_sent;
  int bytes_received;
  
  session(int linenumber, const std::string& source_ipaddr, std::string& dest_ipaddr, int bytes_sent, int bytes_received)
    : linenumber(linenumber), source_ipaddr(source_ipaddr), dest_ipaddr(dest_ipaddr), bytes_sent(bytes_sent), bytes_received(bytes_received) {}

  /*
  void print() const
  { std::cout << linenumber << "," << source_ipaddr << "," << dest_ipaddr << std::endl; }
  */
};

struct linenumber {};
struct source_ipaddr {};
struct dest_ipaddr {};
struct bytes_sent {};
struct bytes_received {};
struct seq {}; // 挿入順のタグ

typedef multi_index_container<
  session,
    indexed_by<
      ordered_unique<tag<linenumber>,   member<session, int, &session::linenumber> >,
      ordered_unique<tag<source_ipaddr>, member<session, std::string, &session::source_ipaddr> >,
      ordered_unique<tag<dest_ipaddr>, member<session, std::string, &session::dest_ipaddr> >,
      ordered_unique<tag<bytes_sent>, member<session, int, &session::bytes_sent> >,
      ordered_unique<tag<bytes_received>, member<session, int, &session::bytes_received> >,
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

std::map<int, int> m_bytes_sent;
std::map<int, int> m_bytes_received; 

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
    
    int map_counter = 0;

    const auto cells = parse_csv(argv[1]);
    AddrParse parser;

    map_counter = 0;
    for (const auto& rows : cells) {

      cout << line_counter << ":" << "bytes_sent: " << rows[21] << endl;
      cout << line_counter << ":" << "bytes_received: " << rows[22] << endl;

      /*
      m_bytes_sent.insert[map_counter] = rows[21];
      m_bytes_received[map_counter] = rows[22];
      */      

      m_bytes_sent.insert(std::make_pair(map_counter, atoi(rows[21].c_str())));
      m_bytes_received.insert(std::make_pair(map_counter, atoi(rows[22].c_str())));
      
      for (const auto& cell : rows) {
	parse_info<string::const_iterator> info =
	  parse( cell.begin(), cell.end(), parser );

	if(info.full) {
	  cout << "line:" << line_counter << " " << cell << ":" << counter << endl;
	  // push_vector(cell);
	}	
      }
      
      line_counter++;
      map_counter++;
    }
       
    ifs.close();

    cout << "Displaying elements..." << endl;

    /*
    for (const auto& [key, value] : m_bytes_sent){
      std::cout << key << " => " << value << "\n";
    }
    */
    
    map_counter = 0;
    int counter = 0;
    for (auto& x:m) {
          // std::cout << x.first << " => " << x.second << std::endl;

          for (auto& y:m) {
	    if(x.first == y.first && x.second != y.second)
	      {
		// std::cout << "*" << x.first << "," << x.second << "," << y.second << ","  << m_bytes_sent[map_counter] << "," << m_bytes_received[map_counter] << std::endl;

		dict.insert(session(x.first, x.second,  y.second, m_bytes_sent[map_counter], m_bytes_received[map_counter]));
		// cout << map_counter << endl;
		
		if(counter%2 ==0)
		  map_counter++;
		counter++;
	      }

	  }
    }

    for (auto & it: dict.get<0>()) {
      std::cout << it.linenumber << "," << it.source_ipaddr << "," << it.dest_ipaddr << "," << it.bytes_sent << "," << it.bytes_received << std::endl;
    }
    
    return 0;
}
