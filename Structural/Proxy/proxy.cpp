#include <iostream>
#include <string>
#include <sstream>
#include <memory>
using namespace std;

struct BankAccount
{
  virtual ~BankAccount() = default;
  virtual void deposit(int amount) = 0;
  virtual void withdraw(int amount) = 0;
};

struct CurrentAccount : BankAccount // 검사
{
  explicit CurrentAccount(const int balance)
    : balance(balance)
  {
  }

  void deposit(int amount) override
  {
    balance += amount;
  }

  void withdraw(int amount) override
  {
    if (amount <= balance) balance -= amount;
  }

  friend ostream& operator<<(ostream& os, const CurrentAccount& obj)
  {
    return os << "balance: " << obj.balance;
  }

private:
  int balance;
};

struct Image
{
  virtual ~Image() = default;
  virtual void draw() = 0;
};

struct Bitmap : Image
{
  Bitmap(const string& filename)
  {
    cout << "Loading image from " << filename << endl;
  }

  void draw() override
  {
    cout << "Drawing image" << endl;
  }
};

struct LazyBitmap : Image
{
  LazyBitmap(const string& filename): filename(filename) {}
  ~LazyBitmap() { delete bmp; }
  void draw() override
  {
    if (!bmp)
      bmp = new Bitmap(filename);
    bmp->draw();
  }

private:
  Bitmap* bmp{nullptr};
  string filename;
};

void draw_image(Image& img)
{
  cout << "About to draw the image" << endl;
  img.draw();
  cout << "Done drawing the image" << endl;
}

void virtual_proxy()
{
  LazyBitmap img{ "pokemon.png" };
  draw_image(img); // 비트맵이 로딩되었는지 안되었는지 확인
  draw_image(img);
}

void smart_pointers()
{
  BankAccount* a = new CurrentAccount(123);
  a->deposit(321);
  delete a;

  // 아래를 shared_ptr<BankAccount>로 만들면 << 연산자가 동작하지 않는다.
  auto b = make_shared<CurrentAccount>(123);

  BankAccount* actual = b.get(); // pointer's own operations on a .
  b->deposit(321); // 객체의 동작은 -> 으로 접근된다.
                   // 노트: 이 표현식은 위의 것과 동일하다.
  cout << *b << endl;
  // no delete

  // see shared_ptr in file structure window
}

struct Pingable
{
  virtual ~Pingable() = default;
  virtual wstring ping(const wstring& message) = 0;
};

struct Pong : Pingable
{
  wstring ping(const wstring& message) override
  {
    return message + L" pong";
  }
};

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace utility;                    // 문자열 변환등 공통 유틸리티
using namespace web;                        // URI등 공통 웹 기능
using namespace web::http;                  // HTTP 관련 공통 기능
using namespace web::http::client;          // HTTP 클라이언트 기능
using namespace concurrency::streams;       // 비동기 스트림

struct RemotePong : Pingable
{
  wstring ping(const wstring& message) override
  {
    wstring result;
    http_client client(U("http://localhost:9149/"));
    uri_builder builder(U("/api/values/"));
    builder.append(message);
    auto task = client.request(methods::GET, builder.to_string())
      .then([=](http_response r)
    {
      return r.extract_string();
    });
    task.wait();
    return task.get();
  }
};

void tryit(Pingable& pp)
{
  wcout << pp.ping(L"ping") << "\n";
}

void communication_proxy()
{
  Pong pp;
  for (int i = 0; i < 3; ++i)
  {
    tryit(pp);
  }
}

// ======== Property Proxy ======================

template <typename T> struct Property
{
  T value;
  Property(const T initialValue)
  {
    *this = initialValue;
  }
  operator T()
  {
    return value;
  }
  T operator =(T newValue)
  {
    return value = newValue;
  }
};

// ===========================================

struct Creature
{
  Property<int> strength{ 10 };
  Property<int> agility{ 5 };
};

void property_proxy()
{
  Creature creature;
  creature.agility = 20;
  cout << creature.agility << endl;
}

int main()
{
  //property_proxy();
  //smart_pointers();
  //virtual_proxy();
  communication_proxy();

  getchar();
  return 0;
}
