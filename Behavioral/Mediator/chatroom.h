#pragma once

struct ChatRoom
{
  vector<Person*> people; // 추가만 한다고 가정한다

  void join(Person* p);
  void broadcast(const string& origin, const string& message);
  void message(const string& origin, const string& who, const string& message);
};
