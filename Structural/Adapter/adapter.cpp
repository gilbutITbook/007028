#include <cstdio>
#include <stack>
#include <vector>
#include <array>
#include <iostream>
using namespace std;

#include <boost/algorithm/string.hpp>

void stack_and_vector()
{
  stack<int, vector<int>> s;
  s.push(123);
  int x = s.top();
  s.pop();
}

class String
{
  string s;
public:
  String(const string& s) : s{s}
  {
  }

  String to_lower_copy() const
  {
    return{ boost::to_lower_copy(s) };
    //string ss{ s };
    //boost::to_lower(ss);
    //return { ss };
  }

  vector<String> split(const string& delimeter = " ") const
  {
    vector<string> parts;
    boost::split(parts, s, boost::is_any_of(delimeter.c_str()),
      boost::token_compress_on);
    return vector<String>(parts.begin(), parts.end());
  }

  size_t get_length() const { return s.length(); }

  __declspec(property(get = get_length)) size_t length;
};

int main()
{
  // �ҹ��ڷ� ���� �� ������.
  // s�� �ҹ��ڿ��� �����Ѵ�.
  //String s{ "Hello   World" };
  
  /*vector<string> parts;
  boost::to_lower(s);
  boost::split(parts, s, boost::is_any_of(" "));
  for (const auto& p : parts)
    cout << "<" << p << ">" << endl;*/

  String s{ u8"Hello   World" };
  auto parts = s.to_lower_copy().split();
  for (const auto& p : parts)
  {
    // ȣȯ �����ϴ�.
    cout << p.get_length() << endl;
    // ȣȯ���� ��������.
    cout << p.length << endl;
  }
  
  getchar();
  return 0;
}
