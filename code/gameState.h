#ifndef GAME_STATE
#define GAME_STATE

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "./http_stuff.h"
#include "./json.hpp"
using namespace std;
using namespace nlohmann;

struct Snake {
  pair<int, int> head;
  vector<pair<int, int> > body;
  int health;
  std::string id;
  int latency;
  int length;
  std::string name;
  std::string shout;
};
struct Board {
  vector<pair<int, int> > food;
  vector<pair<int, int> > hazards;
  int height;
  vector<Snake> snakes;
  int width;
};
struct Game {
  std::string id;
  struct Ruleset {
    std::string name;
    std::string version;
  } ruleset;
  int timeout;
};
struct Turn {
  int turn;
};
struct You {
  Snake snake;
};

#endif /*GAME_STATE*/