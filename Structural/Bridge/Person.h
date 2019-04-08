#pragma once
#include <string>

struct Person
{
  std::string name;

  class PersonImpl;
  PersonImpl *impl; // 브릿지 - 내부 클래스에서는 필요 없다. 변경될 수도 있다.

  Person();
  ~Person();

  void greet();
};

// pimpl 관례는 대부분의 내부 구현을 별도의 클래스로 분리하여 
// 헤더 종속성으로 인한 재컴파일의 필요성을 제거한다.
// 이 방식은 불안정한 바이너리 인테페이스에 대한 의존을 줄여준다.