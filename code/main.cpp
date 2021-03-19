// HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
// JSON credits: https://github.com/nlohmann/json
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <ctime>
#include <chrono>
#include "./http_stuff.h"
#include "./json.hpp"
#include "gameState.h"
#include <pthread.h>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
using namespace nlohmann;

std::string moves[4] = {"up", "down", "left", "right"};
extern int move(Board &board, Game &game, Turn &turn, You &you, std::set<pair<int, int> > &obstacles);
extern void init_data(const json &data, Board &board, Game &game, Turn &turn,
                      You &you, std::set<pair<int, int> > &obstacles);
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
extern int DEBUG_PRINT;

void test_main(void) {
  /* For testing only */
  auto data = R"(
    {"board":{"food":[{"x":8,"y":2},{"x":10,"y":10},{"x":10,"y":4},{"x":6,"y":8},{"x":0,"y":0},{"x":2,"y":8},{"x":0,"y":6},{"x":4,"y":2}],"hazards":[],"height":11,"snakes":[{"body":[{"x":9,"y":10},{"x":9,"y":9},{"x":9,"y":8}],"head":{"x":9,"y":10},"health":91,"id":"gs_kTKcxB9w99BfGqD3rSbjtPhM","latency":"58","length":3,"name":"testsnake","shout":""},{"body":[{"x":1,"y":10},{"x":1,"y":9},{"x":1,"y":8}],"head":{"x":1,"y":10},"health":91,"id":"gs_XrdgvBhQGD34j86TRgFwP3D3","latency":"61","length":3,"name":"testsnake","shout":""},{"body":[{"x":5,"y":10},{"x":5,"y":9},{"x":5,"y":8},{"x":5,"y":7}],"head":{"x":5,"y":10},"health":95,"id":"gs_p4RGYfGGkCfjqBhYqVBRjQKM","latency":"120","length":4,"name":"testsnake","shout":""}],"width":11},"game":{"id":"e437218a-ddd4-486c-a18f-cb0af1ae05bc","ruleset":{"name":"standard","version":"v1.0.17"},"timeout":500},"turn":9,"you":{"body":[{"x":9,"y":10},{"x":9,"y":9},{"x":9,"y":8}],"head":{"x":9,"y":10},"health":91,"id":"gs_kTKcxB9w99BfGqD3rSbjtPhM","latency":"58","length":3,"name":"testsnake","shout":""}}
  )"_json;

  Board board;
  Game game;
  Turn turn;
  You you;
  std::set<pair<int, int> > obstacles;
  init_data(data, board, game, turn, you, obstacles);
  int index = move(board, game, turn, you, obstacles);
}

void run_program(void) {
  httplib::Server svr;
  svr.Get("/", [](const auto &, auto &res) {
    string head = "default";  // TODO: Change head
    string tail = "default";  // TODO: Change tail
    string author = "NaN";
    string color = "#888888";  // TODO: Change a hex color
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head +
                        "\", \"tail\":\"" + tail + "\", \"color\":\"" + color +
                        "\", " + "\"author\":\"" + author + "\"}",
                    "text/json");
  });
  svr.Post("/end", [](const auto &, auto &res) {
    res.set_content("ok", "text/plain");
  });
  svr.Post("/start", [](const auto &, auto &res) {
    res.set_content("ok", "text/plain");
  });
  svr.Post("/move", [](auto &req, auto &res) {
    const json data = json::parse(req.body);
    const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    const boost::posix_time::time_duration td = now.time_of_day();
    const long hours        = td.hours();
    const long minutes      = td.minutes();
    const long seconds      = td.seconds();
    const long milliseconds = td.total_milliseconds() -
                              ((hours * 3600 + minutes * 60 + seconds) * 1000);

    if (DEBUG_PRINT) {
      cout << "====== new snake state " << milliseconds << " ============" << endl;
      pthread_mutex_lock(&print_mutex);
      cout << data;
      cout << "\n\n";
      pthread_mutex_unlock(&print_mutex);
    }

    Board board;
    Game game;
    Turn turn;
    You you;

    std::set<pair<int, int> > obstacles;

    init_data(data, board, game, turn, you, obstacles);
    int index = move(board, game, turn, you, obstacles);
    res.set_content("{\"move\": \"" + moves[index] + "\"}", "text/plain");
  });

  svr.listen("0.0.0.0", 8080);
  std::cout << "Server started";
}

int main(void) {
  // test_main();
  run_program();
  return 0;
}
