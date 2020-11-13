#include <iostream>
#include <cstring> // strcmp

template <class T>
bool equal(T x, T y)
{
  return x == y;
}

template <>
bool equal(const char* x, const char* y)
{
  return std::strcmp(x, y) == 0;
}

int main()
{
  int n = 4;
  const char* s = "Go";
  std::cout << std::boolalpha;
  std::cout << equal(n, 2) << std::endl;
  std::cout << equal(n, 4) << std::endl;
  std::cout << equal(s, "Python") << std::endl;
  std::cout << equal(s, "Go") << std::endl;
}
