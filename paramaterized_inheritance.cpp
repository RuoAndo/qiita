#include <iostream>
#include <string>

using namespace std;

class text
{
public:
  text() {}
  ~text() {}
  string out(const string& str) const { return str; }
};

template <class Base>
class p : public Base {
public:
  string out(const string& str) const { return "<p>" + Base::out(str) + "</p>"; }
};

template <class Base>
class a : public Base {
public:
  string out(const string& str) const { return "<a>" + Base::out(str) + "</a>"; }
};

int main(int argc, char *argv[])
{

  /*
  a<p> hoge0;
  cout << hoge0.out("fuga") << endl;
  */  

  a<text> hoge1;
  cout << hoge1.out("fuga") << endl;

  p<text> hoge2;
  cout << hoge2.out("fuga") << endl;

  p<a<text> > hoge3;
  cout << hoge3.out("fuga") << endl;
  
  a<p<text> > hoge4;
  cout << hoge4.out("fuga") << endl;
  
  // output: <a><p>fuga</p></a>
  return 0;
}

