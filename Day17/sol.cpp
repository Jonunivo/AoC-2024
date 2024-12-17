// STL includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <sstream>
#include <cmath>

using namespace std;
typedef long long LL;

LL Formula(LL n) {
  return ((7 - (n % 8)) ^ (n >> (3 ^ (n % 8)))) % 8;
}

LL Go(LL n, const vector<LL>& goals, int cur) {
  for (int i = 0; i < 8; i++) {
    if (Formula(n + i) == goals[cur]) {
      if (cur + 1 == goals.size()) return n;
      LL ret = Go((n + i) << 3, goals, cur + 1);
      if (ret != -1) return ret;
    }
  }
  return -1;
}

int main() {
  // Hard-coded goals from my input
  vector<LL> goals = { 0,2,4,1,7,7,5,0,3,1,7,4,1,5,5,3,0 };
  std::cout << Go(0, goals, 0) << std::endl;
  return 0;
}