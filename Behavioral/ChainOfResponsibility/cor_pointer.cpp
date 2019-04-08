#include <iostream>
#include <string>
using namespace std;

struct Creature
{
  string name;
  int attack, defense;

  Creature(const string& name, const int attack, const int defense)
    : name(name),
      attack(attack),
      defense(defense)
  {
  }


  friend ostream& operator<<(ostream& os, const Creature& obj)
  {
    return os
      << "name: " << obj.name
      << " attack: " << obj.attack
      << " defense: " << obj.defense;
  }
};

class CreatureModifier
{
  CreatureModifier* next{ nullptr }; // unique_ptr
protected:
  Creature& creature; // 포인터 또는 shared_ptr
public:
  explicit CreatureModifier(Creature& creature)
    : creature(creature)
  {
  }
  virtual ~CreatureModifier() = default;

  void add(CreatureModifier* cm)
  {
    if (next) next->add(cm);
    else next = cm;
  }

  // 두 가지 접근방법:

  // 1. 항상 베이스의 handle()을 호출한다. 추가적인 로직을 수행할 수도 있다.
  // 2. 직접 처리할 수 없을때만 베이스의 handle()을 호출한다.

  virtual void handle()
  {
    if (next) next->handle();
  }
};

// 1. 크리처의 공격력을 두 배 늘린다.
// 2. 힘이 2보다 크지 않으면 방어력을 1 늘린다.
// 3. 이 크리처에는 보너스가 적용되지 않는다.

class NoBonusesModifier : public CreatureModifier
{
public:
  explicit NoBonusesModifier(Creature& creature)
    : CreatureModifier(creature)
  {
  }

  void handle() override
  {
    // 아무것도 안함.
  }
};

class DoubleAttackModifier : public CreatureModifier
{
public:
  explicit DoubleAttackModifier(Creature& creature)
    : CreatureModifier(creature)
  {
  }

  void handle() override
  {
    creature.attack *= 2;
    CreatureModifier::handle();
  }
};

class IncreaseDefenseModifier : public CreatureModifier
{
public:
  explicit IncreaseDefenseModifier(Creature& creature)
    : CreatureModifier(creature)
  {
  }


  void handle() override
  {
    if (creature.attack <= 2)
      creature.defense += 1;
    CreatureModifier::handle();
  }
};

int main_()
{
  Creature goblin{ "Goblin", 1, 1 };
  CreatureModifier root{ goblin };
  DoubleAttackModifier r1{ goblin };
  DoubleAttackModifier r1_2{ goblin };
  IncreaseDefenseModifier r2{ goblin };
  //NoBonusesModifier nb{ goblin }; //유효한 Command 객체

  //root.add(&nb);
  root.add(&r1);
  root.add(&r1_2);
  root.add(&r2);

  root.handle(); // 다소 번거롭다

  cout << goblin << endl;

  //getchar();
  return 0;
}
