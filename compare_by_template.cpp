#include <iostream>
using std::cout; using std::endl;

#include <functional>
using std::less;

#include <cstring>
using std::strcmp;

template <typename T> int compare(const T &v1, const T &v2)
{
	cout << "FUNC1: compare(T)" << "\t";
	cout << " " << v1 << "," << v2 << endl;
	
	if (less<T>()(v1, v2)) return -1;
	if (less<T>()(v2, v1)) return 1;

	return 0;
}

template<unsigned N, unsigned M> 
int compare(const char (&p1)[N], const char (&p2)[M])
{
	cout << "FUNC2: compare(const char arrays)" << "\t";
	cout << " " << p1 << "," << p2 << endl;
	
	return strcmp(p1, p2);
}

template<> 
int compare(const char *const& p1, const char *const& p2)
{
	cout << "FUNC3: compare(const char*const)" << "\t";
	cout << " " << p1 << "," << p2 << endl;
	
	return strcmp(p1, p2);
}

int main()
{
	int *p1 = new int(45);
	int *p2 = new int(42);

	cout << compare(*p1, *p2) << endl;
	
	cout << "Pointer address:" << endl;
	cout << compare(p1, p2) << endl;
	
	cout << "strcmp:" << strcmp("hello", "world!") << endl;
	cout << compare("hello", "world!") << endl;

	const char *cp1 = "hello", *cp2= "world!";
	cout << compare(cp1, cp2) << endl;
	
	const char *cp3 = "hello", *cp4= "hello";
	cout << compare(cp3, cp4) << endl;
	
	return 0;
}
