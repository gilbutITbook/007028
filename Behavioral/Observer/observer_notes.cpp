#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
class Foo;
using namespace std;

// 함수 포인터

// quantlib의 관찰자 패턴

// 컬렉션 변경 vs 멤버 변경

// boost::signal
// 속성 변경 알림
// rxcpp

recursive_mutex mtx;
// 이 뮤텍스를 쓰더라도 데드락에 빠질 수 있다.

struct Listener
{
  virtual void fooChanged(Foo* foo); // const가 아니다
  virtual ~Listener() = default;
};


class Foo
{
  string s;
  vector<Listener*> listeners; // set<>?
  // vector대신 list<>를 사용해도 순회중에 삭제가 가능하다
public:

  virtual string GetS() const
  {
    return s;
  }

  virtual void SetS(const string cs)
  {
    // 여기도 락이 필요할까?
    if (s != cs)
    {
      s = cs;
      notifyListeners();
    }
  }

  void addListener(Listener* l)
  {
    listeners.push_back(l);

    // add에 의해 vector를 재할당해야할 수도 있다. 그러면 재귀적인 add가 실패한다.
  }

  bool removeListener(Listener* l)
  {
    // 여기서 삭제해서는 안된다.
    //remove(listeners.begin(), listeners.end(), l);



    listeners.erase(
      remove(listeners.begin(), listeners.end(), l),
      listeners.end()
    );

    // 대신, 찾은 다음 삭제한다.
    // 첫 번째로 발견된 것만 삭제한다.
    listeners.erase(
      find(listeners.begin(), listeners.end(), l)
    );

    // list에서 발견되는 것이 없으면 어떻게 될까?
    // find()가 end()를 리턴할 것이기 때문에 크래시된다.


    auto it = find(listeners.begin(), listeners.end(), l);
    if (it != listeners.end())
    {
      listeners.erase(it);

      // return true?
    }
    // return false

    // also begin rbegin()/rend()으로도 시작할 수 있을까?
    // 아니다. erase()는 reverse_iterator를 받지 않는다.

    // 도데체 어디를 삭제해야 할까? 메멘토를 리턴해야 할까?
    // 순서를 정하고 약속하는 일들 모두 번거롭다.   

    // 삭제 대신 null로 세팅하는 것은 어떤가?
    for (auto it = listeners.begin(); it != listeners.end(); ++it)
    {
      if (*it == l)
      {
        *it = nullptr; // 첫 번째 항목만 null로 세팅한다.
        // 직접 삭제하는 경우만 처리할 수 있고
        // 외부에서 삭제하는 경우는 방법이 없다.
        return true;
      }
    }

    // 그런데 null 리스너를 남겨두면 나중에 크래시가 발생할 수 있다.
    // null 체크를 해야할까?

    return false;
  }
private:
  void notifyListeners()
  {
    lock_guard<recursive_mutex> lg(mtx); // non-recursive

    if (listeners.empty()) return;

    // 여기서의 명시적인 ++ 연산은 재귀적인 배열 인덱싱에서
    // 크래시를 일으킬 수도 있다.
    int size = listeners.size();
    for (int i = 0; i < size; ++i)
      if (listeners[i])
        listeners[i]->fooChanged(this); // 안전하지 않다.
    // null이 있다면 삭제한다.
    listeners.erase(
      remove(listeners.begin(), listeners.end(), nullptr),
      listeners.end()
    );
    
  }
};

void Listener::fooChanged(Foo* foo)
{
  cout << "foo has been changed to " << foo << "\n";
}

struct ScopeListener : Listener
{
  Foo& foo;

  explicit ScopeListener(Foo& foo)
    : foo(foo)
  {
    startListening();
  }

  ~ScopeListener()
  {
    stopListening();
  }

  void startListening() { foo.addListener(this); }
  void stopListening() { foo.removeListener(this); }


  void fooChanged(Foo* foo) override;
};

void ScopeListener::fooChanged(Foo* foo)
{
  cout << foo->GetS();
  this->stopListening();
}

void cppnow_talk()
{
  // 리스너들은 순서(우선순위?)를 가진다 

  // 같은 리스너를 두 번 추가하는 것도 가능해야 하나?
  // 만약 그래야한다면, 해당 리스너를 삭제해야 할때 내부적으로 2개를
  // 삭제해야 한다. 
  // 항목이 이미 존재한다는 것은 어떻게 알 수 있을까?
  // usage_count 나 set<>을 사용할 수 있다.

  Foo foo;
  ScopeListener sl(foo);

  foo.SetS("abc"); // 크래시가 발생한다.
                   // 리스너 안에서 리스너를 삭제하면 
                   // 순회중인 컬렉션을 변경하게 되어버린다.

  // 그렇게되면 관찰자가 다른 관찰자의 포인터를 가지게된다.
}

int main_test2()
{
  cppnow_talk();

  getchar();
  return 0;
}
