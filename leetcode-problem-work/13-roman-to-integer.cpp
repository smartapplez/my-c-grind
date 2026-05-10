#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Solution {
public:
  int romanToInt(string s) {
    unordered_map<char, int> dict;
    dict['I'] = 1;
    dict['V'] = 5;
    dict['X'] = 10;
    dict['L'] = 50;
    dict['C'] = 100;
    dict['D'] = 500;
    dict['M'] = 1000;

    int roman_sum = 0;
    for (int i = s.size() - 1; i >= 0; i--) {
      if (i != 0 && dict[s.at(i)] > dict[s.at(i - 1)]) {
        roman_sum += dict[s.at(i)] - dict[s.at(i - 1)];
        i--;
      } else {
        roman_sum += dict[s.at(i)];
      }
    }
    return roman_sum;
  }
};

int main() {
  Solution soln;
  string s = "MCDL";
  int roman = soln.romanToInt(s);
  std::cout << s << " -> " << roman << endl;
}
