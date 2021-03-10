//HTTP credits: http://lcs.ios.ac.cn/~maxt/SPelton/reports/report-9aa0d3.html
//JSON credits: https://github.com/nlohmann/json
#include <iostream>
#include "./json.hpp"
#include "./http_stuff.h"
using namespace std;
using namespace nlohmann;

extern string moves;
extern int cycle;

int move(const json data){
  int index = cycle % 4;
  return index;
}