#include <iostream>
#include <string>

using namespace std;

struct debug_log {
  static void display(const std::string& value)
  {
    std::cout << "[debug log]" << value << std::endl;
  }
};

struct release_log {
  static void display(const std::string&) {}
};

template <class LogPolicy = debug_log>
struct huga {
  void foo() const
  {
    LogPolicy::display("Fatal Error!!!");
  }
};

int main()
{

  std::cout << "huga<debug_log>().foo();" << endl;
  huga<debug_log>().foo();
  cout << endl;
  
  cout << "huga<release_log>().foo();" << endl;
  huga<release_log>().foo();
  cout << endl;
  
  cout << "huga<>().foo();" << endl; 
  huga<>().foo();
  cout << endl;
  
  cout << "typedef huga<release_log> release_huga;" << endl;
  cout << "release_huga().foo();" << endl;
  typedef huga<release_log> release_huga;
  release_huga().foo();
  cout << endl;
  
  cout << "typedef huga<debug_log> debug_huga;" << endl;
  cout << "debug_huga().foo();" << endl;
  typedef huga<debug_log> debug_huga;
  debug_huga().foo();
  cout << endl;
}
