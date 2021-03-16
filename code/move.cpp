#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "./http_stuff.h"
#include "./json.hpp"
#include "gameState.h"
using namespace std;
using namespace nlohmann;

std::set<pair<int, int> > obstacles;
bool DEBUG_PRINT = true;

void init_data(const json &data, Board &board, Game &game, Turn &turn,
               You &you) {
  obstacles.clear();
  board.height = data["board"]["height"];
  board.width = data["board"]["width"];
  // init board
  for (int i = 0; i < data["board"]["food"].size(); i++) {
    board.food.push_back(make_pair(data["board"]["food"][i]["x"],
                                   data["board"]["food"][i]["y"]));
  }

  for (int i = 0; i < data["board"]["hazards"].size(); i++) {
    pair<int, int> temp = make_pair(data["board"]["hazards"][i]["x"],
                                    data["board"]["hazards"][i]["y"]);
    board.hazards.push_back(temp);
    obstacles.insert(temp);
  }
  for (int i = 0; i < data["board"]["snakes"].size(); i++) {
    Snake snake;
    // head
    snake.head.first = data["board"]["snakes"][i]["head"]["x"];
    snake.head.second = data["board"]["snakes"][i]["head"]["y"];
    // health
    snake.health = data["board"]["snakes"][i]["health"];
    // id
    snake.id = data["board"]["snakes"][i]["id"];
    // latency
    std::string temp = data["board"]["snakes"][i]["latency"];
    snake.latency = std::stoi(temp);
    // length
    snake.length = data["board"]["snakes"][i]["length"];
    // name
    snake.name = data["board"]["snakes"][i]["name"];
    // shout
    snake.shout = data["board"]["snakes"][i]["shout"];

    for (int j = 0; j < data["board"]["snakes"][i]["body"].size(); j++) {
      pair<int, int> temp =
          make_pair(data["board"]["snakes"][i]["body"][j]["x"],
                    data["board"]["snakes"][i]["body"][j]["y"]);
      snake.body.push_back(temp);
      obstacles.insert(temp);
    }
    board.snakes.push_back(snake);
  }

  // init game
  game.id = data["game"]["id"];
  game.ruleset.name = data["game"]["ruleset"]["name"];
  game.ruleset.version = data["game"]["ruleset"]["version"];
  game.timeout = data["game"]["timeout"];
  // init Turn
  turn.turn = data["turn"];
  // init you
  Snake snake;
  snake.head.first = data["you"]["head"]["x"];
  snake.head.second = data["you"]["head"]["y"];
  snake.health = data["you"]["health"];
  snake.id = data["you"]["id"];
  std::string temp = data["you"]["latency"];
  snake.latency = std::stoi(temp);
  snake.name = data["you"]["name"];
  snake.shout = data["you"]["shout"];
  for (int i = 0; i < data["you"]["body"].size(); i++) {
    pair<int, int> temp =
        make_pair(data["you"]["body"][i]["x"], data["you"]["body"][i]["y"]);
    snake.body.push_back(temp);
    obstacles.insert(temp);
  }
  you.snake = snake;
}

int move(Board &board, Game &game, Turn &turn, You &you) {
  cout << "snake id: " << you.snake.id << ", x: " << you.snake.head.first << ", y: " << you.snake.head.second << endl;
  vector<int> pot_moves;
  if (DEBUG_PRINT) {
    // print obstacles
    cout << "obstacles: ";
    for (auto &e : obstacles) {
      cout << "(" << e.first << ", " << e.second << ") ";
    }
    cout << endl;
  }
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if ((i == 0 && j == 0) || abs(i) == abs(j)) continue;
      if (DEBUG_PRINT) cout << "i: " << i << ", j: " << j << endl;
      pair<int, int> p =
          std::make_pair(you.snake.head.first + i, you.snake.head.second + j);
      if (p.first < 0 || p.first >= board.width || p.second < 0 ||
          p.second >= board.height)
        continue;

      std::set<pair<int, int> >::iterator it = obstacles.find(p);
      if (it != obstacles.end()) continue;
      // {"up", "down", "left", "right"};
      if (DEBUG_PRINT)
        cout << "p.first: " << p.first << ", p.second: " << p.second
             << ", i: " << i << ", j: " << j << endl;
      if (i == -1 && j == 0)
        pot_moves.push_back(2);  // left
      else if (i == 0 && j == 1)
        pot_moves.push_back(0);  // up
      else if (i == 1 && j == 0)
        pot_moves.push_back(3);  // right
      else if (i == 0 && j == -1)
        pot_moves.push_back(1);  // down
      else
        cout << "ERROR not a valid move" << endl;
    }
  }
  vector<std::string> moves{"up", "down", "left", "right"};
  if (DEBUG_PRINT) {
    cout << "potential moves: ";
    for (auto &e : pot_moves) {
      cout << moves[e] << " ";
    }
    cout << endl;
  }
  int t = pot_moves.size();
  if (t <= 0) {
    int k = rand() % 4;
    if (DEBUG_PRINT) cout << "move: " << moves[k] << endl;
    return k;
  }
  int r = rand() % t;
  if (DEBUG_PRINT) {
    cout << "move: " << moves[pot_moves[r]] << endl;
    cout << endl;
  }
  return pot_moves[r];
}