#include <boost/spirit/include/qi.hpp>
#include <string>
#include <iostream>

using namespace boost::spirit;

int main()
{
  std::string str;
  std::getline(std::cin, str);
  auto it = str.begin();
  bool match = qi::phrase_parse(it, str.end(),
    qi::lexeme[ascii::digit >> ascii::digit], ascii::space);
  std::cout << std::boolalpha << match << '\n';
  if (it != str.end())
    std::cout << std::string{it, str.end()} << '\n';
}

