#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "./http_stuff.h"
#include "./json.hpp"
#include "gameState.h"
using namespace std;
using namespace nlohmann;

extern std::string moves;
extern int cycle;

void init_data(const json data) {
  // init board
  Board board;
  for (int i = 0; i < data["board"]["food"].size(); i++) {
    board.food.push_back(make_pair(data["board"]["food"][i]["x"],
                                   data["board"]["food"][i]["y"]));
  }

  for (int i = 0; i < data["board"]["hazards"].size(); i++) {
    board.hazards.push_back(make_pair(data["board"]["hazards"][i]["x"],
                                      data["board"]["hazards"][i]["y"]));
  }

  board.height = data["board"]["height"];
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
      snake.body.push_back(
          make_pair(data["board"]["snakes"][i]["body"][j]["x"],
                    data["board"]["snakes"][i]["body"][j]["y"]));
    }
    board.snakes.push_back(snake);
  }

  board.width = data["board"]["width"];
  // init game
  Game game;
  game.id = data["game"]["id"];
  game.ruleset.name = data["game"]["ruleset"]["name"];
  game.ruleset.version = data["game"]["ruleset"]["version"];
  game.timeout = data["game"]["timeout"];
  // init Turn
  Turn turn;
  turn.turn = data["turn"];
  // init you
  You you;
  Snake snake;
  snake.head.first = data["you"]["head"]["x"];
  snake.head.second = data["you"]["head"]["y"];
  snake.health = data["you"]["health"];
  snake.id = data["you"]["id"];
  std::string temp = data["you"]["latency"];
  snake.latency = std::stoi(temp);
  snake.name = data["you"]["name"];
  snake.name = data["you"]["shout"];
  for (int i = 0; i < data["you"]["body"].size(); i++) {
    snake.body.push_back(
        make_pair(data["you"]["body"][i]["x"], data["you"]["body"][i]["y"]));
  }
  you.snake = snake;
}

int move(Board &board, Game &game, Turn &turn, You &you) {
  int index = cycle % 4;
  return index;
}