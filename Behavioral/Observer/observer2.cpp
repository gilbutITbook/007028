#include <string>
#include <vector>
#include <iostream>
#include <mutex>
using namespace std;
#include <boost/any.hpp>
using namespace boost;

namespace {

struct Person;

struct PersonListener
{
  virtual ~PersonListener() = default;
  virtual void person_changed(Person& p, const string& property_name, const any new_value) = 0;
};

static mutex mtx;

struct Person
{
  explicit Person(int age)
    : age(age)
  {
  }

  int get_age() const
  {
    return age;
  }

  void set_age(const int age)
  {
    if (this->age == age) return;

    auto old_c_v = get_can_vote();

    this->age = age;
    notify("age", this->age);

    auto new_c_v = get_can_vote();
    if (old_c_v != new_c_v)
    {
      notify("can_vote", new_c_v);
    }
  }

  bool get_can_vote() const
  {
    return age >= 16;
  }

  void subscribe(PersonListener* pl)
  {
    lock_guard<mutex> guard{ mtx };

    // 기 존재하는 참조를 방지해야 할까?
    if (find(begin(listeners), end(listeners), pl) == end(listeners))
      listeners.push_back(pl);
  }
  void ubsubscribe(PersonListener* pl)
  {
    lock_guard<mutex> guard{ mtx };
    if (listeners.empty()) return;
    // 복수의 동일한 리스너들?
    // 삭제하고 이동하기 관례?
    for (auto it = listeners.begin(); it != listeners.end(); ++it)
    {
      if (*it == pl)
      {
        *it = nullptr; // 그냥 nullptr로 표시한다
      }
    }
  }
private:
  int age;
  vector<PersonListener*> listeners;

  void notify(const string& property_name, const any new_value)
  {
    lock_guard<mutex> guard{ mtx };
    for (const auto listener : listeners)
      if (listener)
        listener->person_changed(*this, property_name, new_value);

    // 삭제하고 이동하기
    listeners.erase(
      remove(listeners.begin(), listeners.end(), nullptr),
      listeners.end()
    );
  }

  // std::list (항목의 삭제가 더 쉽다)
  // concurrent_vector는 어떠한가? 가능하다. 
  // 하지만 삭제하고 이동하기 관례의 적용이 어렵다.
};

struct ConsoleListener : PersonListener
{
  void person_changed(Person& p, const string& property_name, 
    const any new_value) override
  {
    cout << "person's " << property_name << " has been changed to ";
    if (property_name == "age")
    {
      cout << any_cast<int>(new_value);
    }
    else if (property_name == "can_vote")
    {
      cout << any_cast<bool>(new_value);
    }
    cout << "\n";
  }
};

int main_test4()
{
  Person p{ 14 };
  ConsoleListener cl;
  p.subscribe(&cl);
  p.subscribe(&cl); // 무시된다
  p.set_age(15);
  p.set_age(16);
  p.ubsubscribe(&cl);
  p.set_age(17);


  getchar();
  return 0;
}

}