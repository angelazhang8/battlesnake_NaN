//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include <vector>
#include "./http_stuff.h"
using namespace std;
using namespace nlohmann;

extern string moves;
extern int cycle;

int init_data(const json data){
  cout << "\n\n" << data["board"]["food"]["x"] << "\n\n";
  vector<int>food;
  
  // for (const auto& item : data["board"]["food"].items())
  //   {
  //      food.push_back(item);
  //   }
  // for (auto &f : food){
  //   cout << f << endl;
  // }
  return 0;
}

int move(){
  // cout << "data: " << data[1] << "\n";
  int index = cycle % 4;
  return index;
}