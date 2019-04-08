#include <iostream>
#include <string>
#include <vector>
struct Game;
using namespace std;

#include <boost/signals2.hpp>
using namespace boost::signals2;

struct EventData
{
  virtual ~EventData() = default;
  virtual void print() const = 0;
};

struct Player;
struct PlayerScoredData : EventData
{
  string player_name;
  int goals_scored_so_far;

  PlayerScoredData(const string& player_name, const int goals_scored_so_far)
    : player_name(player_name),
      goals_scored_so_far(goals_scored_so_far)
  {
  }

  void print() const override
  {
    cout << player_name << " has scored! (their " 
      << goals_scored_so_far << " goal)" << "\n";
  }
};

struct Game
{
  signal<void(EventData*)> events; // 관찰자
};

struct Player
{
  string name;
  int goals_scored = 0;
  Game& game;


  Player(const string& name, Game& game)
    : name(name),
      game(game)
  {
  }

  void score()
  {
    goals_scored++;
    PlayerScoredData ps{name, goals_scored};
    game.events(&ps);
  }
};

struct Coach
{
  Game& game;

  explicit Coach(Game& game)
    : game(game)
  {
    // 선수가 3점 미만의 득점을 했으면 격려해준다
    game.events.connect([](EventData* e)
    {
      PlayerScoredData* ps = dynamic_cast<PlayerScoredData*>(e);
      if (ps && ps->goals_scored_so_far < 3)
      {
        cout << "coach says: well done, " << ps->player_name << "\n";
      }
    });
  }
};

int main()
{
  Game game;
  Player player{ "Sam", game };
  Coach coach{ game };

  player.score();
  player.score();
  player.score(); // ignored by coach

  getchar();
  return 0;
}
