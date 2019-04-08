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

  // ���丮 �޼��带 �̿��Ѵ�
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
  // ���� �ڵ�� �������� �ʴ´�.
  // Point p{ 1,2 };

  // �̷�...!
  // Point::PointFactory pf;

  // ���� ���丮�� public �̶�� �Ʒ��� ���� �� �� �ִ�
  //auto p = Point::PointFactory::NewCartesian(3, 4);

  // � ��ǥ�� ������ ���� ����� �̿��Ѵ�
  auto pp = Point::Factory.NewCartesian(2, 3);

  return 0;
}