#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <pthread.h>
#include "./http_stuff.h"
#include "./json.hpp"
#include "gameState.h"
using namespace std;
using namespace nlohmann;

bool DEBUG_PRINT = false;
bool SEG_PRINT = false;
extern pthread_mutex_t print_mutex;

void init_data(const json &data, Board &board, Game &game, Turn &turn,
               You &you, std::set<pair<int, int> > &obstacles) {
  
  if (!data.contains("board")) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  if (!data["board"].contains("height") || !data["board"]["height"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  board.height = data["board"]["height"].get<int>();

  if (SEG_PRINT) cout << "board.height: " << board.height << endl << std::flush;

  if (!data["board"].contains("width") || !data["board"]["width"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  board.width = data["board"]["width"].get<int>();

  if (SEG_PRINT) cout << "board.width: " << board.width << endl << std::flush;

  // init board
  if (!data["board"].contains("food") || !data["board"]["food"].is_array()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  for (int i = 0; i < data["board"]["food"].size(); i++) {
    board.food.push_back(make_pair(data["board"]["food"][i]["x"].get<int>(),
                                   data["board"]["food"][i]["y"].get<int>()));
  }
  if (SEG_PRINT){
    cout << "board.food: ";
    for (auto &e : board.food){
      cout << "(" << e.first << ", " << e.second << ") ";
    }
    cout << endl << std::flush;
  }
  // for (int i = 0; i < data["board"]["hazards"].size(); i++) {
  //   pair<int, int> temp = make_pair(data["board"]["hazards"][i]["x"],
  //                                   data["board"]["hazards"][i]["y"]);
  //   board.hazards.push_back(temp);
  //   obstacles.insert(temp);
  // }
  // if (SEG_PRINT){
  //   cout << "board.hazards: ";
  //   for (auto &e : board.hazards){
  //     cout << "(" << e.first << ", " << e.second << ") ";
  //   }
  //   cout << endl << std::flush;
  // }
  if (!data["board"].contains("snakes") || !data["board"]["snakes"].is_array()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  for (int i = 0; i < data["board"]["snakes"].size(); i++) {
    // head
    Snake snake;
    auto &inp = data["board"]["snakes"][i];

    if (!inp.contains("head") || !inp["head"].is_object()) { if (DEBUG_PRINT) cout << "error" << endl; } 
    if (!inp["head"].contains("x") || !inp["head"]["x"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    if (!inp["head"].contains("y") || !inp["head"]["y"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    snake.head.first = inp["head"]["x"].get<int>();
    snake.head.second = inp["head"]["y"].get<int>();

    // health
    if (!inp.contains("health") || !inp["health"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    snake.health = inp["health"].get<int>();

    // id
    if (!inp.contains("id") || !inp["id"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    snake.id = inp["id"].get<std::string>();

    // latency
    if (!inp.contains("latency") || !inp["latency"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    std::string temp = inp["latency"].get<std::string>();
    snake.latency = std::stoi(temp);

    // length
    if (!inp.contains("length") || !inp["length"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    snake.length = inp["length"].get<int>();

    // name
    if (!inp.contains("name") || !inp["name"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    snake.name = inp["name"].get<std::string>();

    // shout
    if (!inp.contains("shout") || !inp["shout"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    snake.shout = inp["shout"].get<std::string>();

    if (!inp.contains("body") || !inp["body"].is_array()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    for (int j = 0; j < inp["body"].size(); j++) {
      auto &bod =  inp["body"][j];
      if (!bod.contains("x") || !bod["x"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
      if (!bod.contains("y") || !bod["y"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 

      snake.body.push_back(make_pair(bod["x"].get<int>(), bod["y"].get<int>()));
      obstacles.insert(make_pair(bod["x"].get<int>(), bod["y"].get<int>()));
    }
    board.snakes.push_back(snake);
  }
  if (SEG_PRINT){
    cout << "------ board.snakes --------------------- " << endl;
    for (auto &snake : board.snakes){
      cout << "snake.head: (" << snake.head.first << ", " << snake.head.second << "),\n" << std::flush;
      cout << "snake.health: " << snake.health << endl << std::flush;
      cout << "snake.id: " << snake.id << endl << std::flush;
      cout << "snake.latency: " << snake.latency << endl << std::flush;
      cout << "snake.length: " << snake.length << endl << std::flush;
      cout << "snake.name: " << snake.name << endl << std::flush;
      cout << "snake.shout: " << snake.shout << endl << std::flush;
      cout << endl << "board.snakes.snake.body: ";
      for (auto &e: snake.body){
        cout << "(" << e.first << ", " << e.second << ") ";
      }
      cout << endl << std::flush;
    }
  }
  // init game
  game.id = data["game"]["id"].get<std::string>();
  if (SEG_PRINT) cout << "game.id: " << game.id << endl << std::flush;
  game.ruleset.name = data["game"]["ruleset"]["name"].get<std::string>();
  if (SEG_PRINT) cout << "game.ruleset.name: " << game.ruleset.name << endl << std::flush;
  game.ruleset.version = data["game"]["ruleset"]["version"].get<std::string>();
  if (SEG_PRINT) cout << "game.ruleset.version: " << game.ruleset.version << endl << std::flush;
  game.timeout = data["game"]["timeout"].get<int>();
  if (SEG_PRINT) cout << "game.timeout: " << game.timeout << endl << std::flush;

  // init Turn
  turn.turn = data["turn"].get<int>();
  if (SEG_PRINT) cout << "turn.turn: " << turn.turn << endl << std::flush;

  // init you
  if (!data.contains("you") || !data["you"].is_object()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; }
  auto &inp = data["you"];
  if (!inp.contains("head") || !inp["head"].is_object()) { if (DEBUG_PRINT) cout << "error" << endl; } 
    if (!inp["head"].contains("x") || !inp["head"]["x"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    if (!inp["head"].contains("y") || !inp["head"]["y"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 

  Snake snake; 
  snake.head.first = data["you"]["head"]["x"].get<int>();
  snake.head.second = data["you"]["head"]["y"].get<int>();
  if (SEG_PRINT) cout << "you.snake.head: (" << you.snake.head.first << ", " << you.snake.head.second << "),\n" << std::flush;

  if (!inp.contains("health") || !inp["health"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  snake.health = data["you"]["health"].get<int>();
  if (SEG_PRINT) cout << "you.snake.health: " << you.snake.health << "\n" << std::flush;

  if (!inp.contains("id") || !inp["id"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  snake.id = data["you"]["id"].get<std::string>();
  if (SEG_PRINT) cout << "you.snake.id: " << you.snake.id << "\n" << std::flush;
  
  if (!inp.contains("latency") || !inp["latency"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  std::string temp = data["you"]["latency"].get<std::string>();
  snake.latency = std::stoi(temp);
  if (SEG_PRINT) cout << "you.snake.latency: " << you.snake.latency << "\n" << std::flush;
  
  if (!inp.contains("length") || !inp["length"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  snake.length = data["you"]["length"].get<int>();
  if (SEG_PRINT) cout << "you.snake.length: " << you.snake.length << "\n" << std::flush;

  if (!inp.contains("name") || !inp["name"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  snake.name = data["you"]["name"].get<std::string>();
  if (SEG_PRINT) cout << "you.snake.name: " << you.snake.name << "\n" << std::flush;

  if (!inp.contains("shout") || !inp["shout"].is_string()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
  snake.shout = data["you"]["shout"].get<std::string>();
  if (SEG_PRINT) cout << "you.snake.shout: " << you.snake.shout << "\n" << std::flush;

  if (!inp.contains("body") || !inp["body"].is_array()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; }
  for (int i = 0; i < data["you"]["body"].size(); i++) {
    auto &bod =  inp["body"][i];
    if (!bod.contains("x") || !bod["x"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 
    if (!bod.contains("y") || !bod["y"].is_number()) { if (DEBUG_PRINT) cout << "error " << __LINE__ << endl; } 

    snake.body.push_back(make_pair(data["you"]["body"][i]["x"], data["you"]["body"][i]["y"].get<int>()));
    obstacles.insert(make_pair(data["you"]["body"][i]["x"], data["you"]["body"][i]["y"].get<int>()));
  }
  you.snake = snake;

  if (SEG_PRINT){
    cout << endl << "you.snake.body: ";
    for (auto &e: you.snake.body){
      cout << "(" << e.first << ", " << e.second << ") ";
    }
    cout << "\n\n" << std::flush;
  }
}

// returns true if there is threat of head to head collision 
bool detectHeadCollision(Board board, int ourLength, pair<int, int> ourNextLocation) {
  for (auto &snake: board.snakes) {
    if (snake.head.second == ourNextLocation.second) {
      if (snake.head.first + 1 == ourNextLocation.first || 
      snake.head.first - 1 == ourNextLocation.first) return true;
    } 
    else if (snake.head.first == ourNextLocation.first){
      if (snake.head.second + 1 == ourNextLocation.second ||
      snake.head.second - 1 == ourNextLocation.second) return true;
    }
  }
  return false;
}

int move(Board &board, Game &game, Turn &turn, You &you, std::set<pair<int, int> > &obstacles) {
  if (DEBUG_PRINT){
    pthread_mutex_lock(&print_mutex);
    cout << "------ start move -------------------------" << endl;
    cout << "snake id: " << you.snake.id << ", x: " << you.snake.head.first << ", y: " << you.snake.head.second << endl;
    pthread_mutex_unlock(&print_mutex);
  }
  vector<int> pot_moves;
  if (DEBUG_PRINT) {
    // print obstacles
    cout << "obstacles: ";
    for (auto &e : obstacles) {
      cout << "(" << e.first << ", " << e.second << ") ";
    }
    cout << endl << std::flush;
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

      // head to head collisions
      // check adjacent squares of our snake's head
      if (detectHeadCollision(board, you.snake.length, p)) continue;

      // {"up", "down", "left", "right"};
      if (DEBUG_PRINT)
        cout << "p.first: " << p.first << ", p.second: " << p.second
             << ", i: " << i << ", j: " << j << endl << std::flush;
      if (i == -1 && j == 0)
        pot_moves.push_back(2);  // left
      else if (i == 0 && j == 1)
        pot_moves.push_back(0);  // up
      else if (i == 1 && j == 0)
        pot_moves.push_back(3);  // right
      else if (i == 0 && j == -1)
        pot_moves.push_back(1);  // down
      else
        cout << "ERROR not a valid move" << endl << std::flush;
    }
  }
  vector<std::string> moves{"up", "down", "left", "right"};
  if (DEBUG_PRINT) {
    pthread_mutex_lock(&print_mutex);
    cout << "potential moves: ";
    for (auto &e : pot_moves) {
      cout << moves[e] << " ";
    }
    cout << endl << std::flush;
    pthread_mutex_unlock(&print_mutex);
  }
  int t = pot_moves.size();
  if (t <= 0) {
    int k = rand() % 4;
    if (DEBUG_PRINT) cout << "move: " << moves[k] << endl << std::flush;
    return k;
  }
  int r = rand() % t;
  if (DEBUG_PRINT) {
    pthread_mutex_lock(&print_mutex);
    cout << "move: " << moves[pot_moves[r]] << endl;
    cout << endl << std::flush;
    pthread_mutex_unlock(&print_mutex);
  }

  return pot_moves[r];
}