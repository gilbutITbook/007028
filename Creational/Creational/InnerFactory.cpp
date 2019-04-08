#include <cmath>

// do not need this for factory
enum class PointType
{
  cartesian,
  polar
};

class Point
{
  /*Point(float a, float b, PointType type = PointType::cartesian)
  {
  if (type == PointType::cartesian)
  {
  x = a; b = y;
  }
  else
  {
  x = a*cos(b);
  y = a*sin(b);
  }
  }*/

  // 펙토리 메서드를 이용한다
  Point(float x, float y) : x(x), y(y) {}

  class PointFactory
  {
    PointFactory() {}
  public:
    static Point NewCartesian(float x, float y)
    {
      return { x,y };
    }
    static Point NewPolar(float r, float theta)
    {
      return{ r*cos(theta), r*sin(theta) };
    }
  };
public:
  float x, y;
  static PointFactory Factory;
};

int main_2()
{
  // 다음 코드는 동작하지 않는다.
  // Point p{ 1,2 };

  // 이런...!
  // Point::PointFactory pf;

  // 만약 펙토리가 public 이라면 아래와 같이 할 수 있다
  //auto p = Point::PointFactory::NewCartesian(3, 4);

  // 어떤 좌표든 다음과 같은 방식을 이용한다
  auto pp = Point::Factory.NewCartesian(2, 3);

  return 0;
}