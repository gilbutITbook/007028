#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

struct Shape
{
  virtual string str() const = 0;
};

struct Circle : Shape
{
  float radius;

  Circle(){}

  explicit Circle(const float radius)
    : radius{radius}
  {
  }

  void resize(float factor)
  {
    radius *= factor;
  }

  string str() const override
  {
    ostringstream oss;
    oss << "A circle of radius " << radius;
    return oss.str();
  }
};

struct Square : Shape
{
  float side;

  Square(){}

  explicit Square(const float side)
    : side{side}
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << "A square with side " << side;
    return oss.str();
  }
};

// �̹� �����ϴ� ��ü�� ���̽� Ŭ������ �����ϴ� ���� �ƴϴ�.
// ���� ��� ColoredSquare, ColoredCircle ���� �͵��� ���� �� ����.

struct ColoredShape : Shape
{
  Shape& shape;
  string color;

  ColoredShape(Shape& shape, const string& color)
    : shape{shape},
      color{color}
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << shape.str() << " has the color " << color;
    return oss.str();
  }
};

struct TransparentShape : Shape
{
  Shape& shape;
  uint8_t transparency;


  TransparentShape(Shape& shape, const uint8_t transparency)
    : shape{shape},
      transparency{transparency}
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << shape.str() << " has "
      << static_cast<float>(transparency) / 255.f*100.f
      << "% transparency";
    return oss.str();
  }
};

// �ͽ���(mixin) ���

// ��Ʈ: typename�� �ƴ϶� class �̴�
template <typename T> struct ColoredShape2 : T
{
  static_assert(is_base_of<Shape, T>::value,
    "Template argument must be a Shape");

  string color;

  // need this (or not!)
  ColoredShape2(){}

  template <typename...Args>
  ColoredShape2(const string& color, Args ...args)
    : T(std::forward<Args>(args)...), color{color}
    // ���⼭ ���̽� Ŭ������ �Ҹ��ڸ� ȣ���� ���� ����.
    // �ֳĸ� �Ҹ��ڸ� �� ���� ���� �����̴�.
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << T::str() << " has the color " << color;
    return oss.str();
  }
};

template <typename T> struct TransparentShape2 : T
{
  uint8_t transparency;

  template<typename...Args>
  TransparentShape2(const uint8_t transparency, Args ...args)
    : T(std::forward<Args>(args)...), transparency{ transparency }
  {
  }

  string str() const override
  {
    ostringstream oss;
    oss << T::str() << " has "
      << static_cast<float>(transparency) / 255.f * 100.f
      << "% transparency";
    return oss.str();
  }
};

void wrapper()
{
  Circle circle{ 5 };
  cout << circle.str() << endl;

  ColoredShape red_circle{ circle, "red" };
  cout << red_circle.str() << endl;

  //red_circle.resize(); // �̷�!

  TransparentShape red_half_visible_circle{ red_circle, 128 };
  cout << red_half_visible_circle.str() << endl;
}

void mixin_inheritance()
{
  // ������ ������ ���̴� �������� �ʴ´�.
  ColoredShape2<Circle> green_circle{ "green", 5 };

  cout << green_circle.str() << endl;

  TransparentShape2<ColoredShape2<Square>> blue_invisible_square{ 0, "blue", 10 };
  blue_invisible_square.color = "blue";
  blue_invisible_square.side = 321;
  cout << blue_invisible_square.str() << endl;
}

struct Logger
{
  std::function<void()> func;
  string name;

  Logger(const std::function<void()>& func, const string& name)
    : func{func},
      name{name}
  {
  }

  void operator()() const
  {
    cout << "Entering " << name << endl;
    func();
    cout << "Exiting " << name << endl;
  }
};

template <typename Func>
struct Logger2
{
  Func func;
  string name;

  Logger2(const Func& func, const string& name)
    : func{func},
      name{name}
  {
  }

  void operator()() const
  {
    cout << "Entering " << name << endl;
    func();
    cout << "Exiting " << name << endl;
  }
};

template <typename Func> auto make_logger2(Func func, 
  const string& name)
{
  return Logger2<Func>{ func, name }; 
}

// ���� �ڵ尡 ����� �����ϱ� ���ؼ��� �κ����� Ư��ȭ�� �ʿ��ϴ�.
template <typename> struct Logger3;

// ���� Ÿ�԰� ���� ���
template <typename R, typename... Args> 
struct Logger3<R(Args...)>
{
  Logger3(function<R(Args...)> func, const string& name)
    : func{func},
      name{name}
  {
  }

  R operator() (Args ...args)
  {
    cout << "Entering " << name << endl;
    R result = func(args...);
    cout << "Exiting " << name << endl;
    return result;
  }

  function<R(Args ...)> func;
  string name;
};

template <typename R, typename... Args>
auto make_logger3(R (*func)(Args...), const string& name)
{
  return Logger3<R(Args...)>(
    std::function<R(Args...)>(func), 
    name);
}

double add(double a, double b)
{
  cout << a << "+" << b << "=" << (a + b) << endl;
  return a + b;
}

void function_decorator()
{
  //Logger([]() {cout << "Hello" << endl; }, "HelloFunction")();
  
  // ���� �ڵ�� ������ �� ����.
  //make_logger2([]() {cout << "Hello" << endl; }, "HelloFunction")();
  auto call = make_logger2([]() {cout << "Hello!" << endl; }, "HelloFunction");
  call();

  auto logged_add = make_logger3(add, "Add");
  auto result = logged_add(2, 3);
}

void constructor_forwarding()
{
  struct NotAShape
  {
    virtual string str() const { return string{}; }
  };

  // �Ʒ��� �ڵ尡 ����Ǿ�� �ȵȴ�. ���� static_assert�� ���´�.
  //ColoredShape2<NotAShape> legal;

  // �� ��� �ڵ� ���ø����� �������� �ʴ´�.
  // ���ڸ� �ּ�ó�� �� ���� �ִ�(����Ʈ ������).
  TransparentShape2<Square> hidden_square{ 1, 2 };
  cout << hidden_square.str() << endl;

  ColoredShape2<TransparentShape2<Square>> sq = { "red", 51, 5 };
  cout << sq.str() << endl;
}

int main()
{
  function_decorator();
  //wrapper();
  //mixin_inheritance();
  //constructor_forwarding();

  getchar();
  return 0;
}
