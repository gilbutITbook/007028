#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

// A. 상위 모듈이 하위 모듈에 종속되어서는 안된다.
//    둘 다 추상화에 종속되어야 한다.
// B. 추상화는 상세한 부분에 종속되어서는 안된다.
//    상세 사항이 추상화에 종속되어야 한다.

enum class Relationship
{
  parent,
  child,
  sibling
};

struct Person
{
  string name;
};

struct RelationshipBrowser
{
  virtual vector<Person> find_all_children_of(const string& name) = 0;
};

struct Relationships : RelationshipBrowser // low-level
{
  vector<tuple<Person, Relationship, Person>> relations;

  void add_parent_and_child(const Person& parent, const Person& child)
  {
    relations.push_back({parent, Relationship::parent, child});
    relations.push_back({child, Relationship::child, parent});
  }

  vector<Person> find_all_children_of(const string &name) override
  {
    vector<Person> result;
    for (auto&& [first, rel, second] : relations)
    {
      if (first.name == name && rel == Relationship::parent)
      {
        result.push_back(second);
      }
    }
    return result;
  }
};

struct Research // high-level
{
  Research(RelationshipBrowser& browser)
  {
    for (auto& child : browser.find_all_children_of("John"))
    {
      cout << "John has a child called " << child.name << endl;
    }
  }
//  Research(const Relationships& relationships)
//  {
//    auto& relations = relationships.relations;
//    for (auto&& [first, rel, second] : relations)
//    {
//      if (first.name == "John" && rel == Relationship::parent)
//      {
//        cout << "John has a child called " << second.name << endl;
//      }
//    }
//  }
};

int main_dip()
{
  Person parent{"John"};
  Person child1{"Chris"};
  Person child2{"Matt"};

  Relationships relationships;
  relationships.add_parent_and_child(parent, child1);
  relationships.add_parent_and_child(parent, child2);

  Research _(relationships);

  return 0;
}