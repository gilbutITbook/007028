#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct ChatRoom;

struct Person
{
  string name;
  ChatRoom* room = nullptr;

  Person(const string& name);
  void receive(const string& origin, const string& message);

  void say(const string& message) const;
  vector<string> chat_log;

  void pm(const string& who, const string& message) const;

  // IDE에서 자동으로 생성된 코드이다
  friend bool operator==(const Person& lhs, const Person& rhs)
  {
    return lhs.name == rhs.name;
  }

  friend bool operator!=(const Person& lhs, const Person& rhs)
  {
    return !(lhs == rhs);
  }
};
