#include <iostream>
using namespace std;
struct widget {
  typedef int arg_type;
};

template <class T>
void something(typename T::arg_type*)
{
  cout << "something : arg_typeを持っている" << endl;
}

template <class T>
void something(...)
{
  cout << "something : arg_typeを持っていない" << endl;
  cout << "なので、それ以外" << endl;
}

int main()
{
  something<widget>(0);
  something<int>(0);
}

