#include <iostream>
#include <string>

using namespace std;

struct debug_log {
  static void display(const std::string& value)
  {
    std::cout << value << std::endl;
  }
};

struct release_log {
  static void display(const std::string&) {}
};

template <class LogPolicy>
struct huga {
  void foo() const
  {
    LogPolicy::display("Error. Need Checking !!!");
  }
};

int main()
{
  cout << "huga<release_log>().foo();" << endl;
  huga<release_log>().foo();
  cout << endl;
  
  cout << "huga<debug_log>().foo();" << endl;
  huga<debug_log>().foo();
  cout << endl;

}
