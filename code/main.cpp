//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include <fstream>
#include "./json.hpp"
#include "./http_stuff.h"
using namespace std;
using namespace nlohmann;

int counter = 0;
string moves[4] = {"up", "down", "left", "right"};

int init_data(const json data);
int cycle = 0;

int main(void) {
  /* For testing only */
  const json data = {
    {"board",
        {"food",
        {{"x",0,"y",0},
          {"x",5,"y",5},
          {"x",9,"y",1},
          {"x",10,"y",1}}},
        {"hazards",{}},
        {"height",11},
        {"snakes",
          {"body",
            {{"x",1,},
            {"y",10},
            {"x",1,"y",9},
            {"x",1,"y",8}}},
          {"head",{"x",1,"y",10}},
          {"health",91},
          {"id","gs_pMjyVgp4FHkVmShkmpWV6SHW"},
          {"latency","64"},
          {"length",3},
          {"name","nicole"},
          {"shout",""},
          {"width",11}}},
    {"game",
      {"id","dee22316-c8fc-46ab-a772-41eb6e85f741","ruleset"},
      {"name","solo","version","v1.0.17"},
      {"timeout",500}},
    {"turn",9},
    {"you",
      {"body",
        {{"x",1,"y",10},
        {"x",1,"y",9},
        {"x",1,"y",8}}},
      {"head",{"x",1,"y",10}},
      {"health",91},
      {"id","gs_pMjyVgp4FHkVmShkmpWV6SHW"},
      {"latency","64"},
      {"length",3},
      {"name","nicole"},
      {"shout",""}}};
  
  int index = init_data(data);
  return 0;
} 
/*
  httplib::Server svr;
  svr.Get("/", [](const auto &, auto &res) {
    string head = "default"; //TODO: Change head
    string tail = "default"; //TODO: Change tail
    string author = "NaN";
    string color = "#888888";  //TODO: Change a hex color
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + head + "\", \"tail\":\"" + tail + "\", \"color\":\"" + color + "\", " + "\"author\":\"" + author + "\"}", "text/json");
  });
  svr.Post("/end", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });
  svr.Post("/start", [](const auto &, auto &res){
    res.set_content("ok", "text/plain");
  });
  svr.Post("/move", [](auto &req, auto &res){
    // const json data;
    // std::ifstream data_file("test.json", std::ifstream::binary);
    // data_file >> data;

    const json data = json::parse(req.body);
    // cout << data;
    // cout << "\n\n";
    //You can get the "you" property like this:
    //data["you"];
    //Almost alike python dictionary parsing, but with a semicolon at the end of each line.
    //You might need to make some structs to store some data in a variable
    //Example:
    //you_struct you = data["you"];
    
    // int index = move(data);
    res.set_content("{\"move\": \"" + moves[index] + "\"}", "text/plain");
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Server started";
}
*/