#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

#include "words.h"

using namespace std;

const size_t MAXLEN = 80;

int words(const char* str, size_t len) {
  int ans = 0;
  bool was_prev_space = true;
  for (size_t i = 0; i < len; ++i) {
    if (str[i] == ' ') {
      was_prev_space = true;
    } else {
      if (was_prev_space) {
        ++ans;
      }
      was_prev_space = false;
    }
  }
  return ans;
}

string normalizeString(const char* str, size_t len) {
  string ret = "\"";
  ret += string(str, min(MAXLEN, len));

  if (MAXLEN < len) {
    ret += "...";
  }
  ret += "\"";
  return ret;
}

bool test(const char* str, size_t len, const string& tag) {
  cout << "Testing " << tag << ": " << normalizeString(str, len) << endl;

  int aCorrect = words(str, len);
  int aTesting = nasm::words(str, len);
  cout << "Correct: " << aCorrect << endl;
  cout << "Testing: " << aTesting << endl;
  if (aCorrect != aTesting) {
    cout << "FAIL!" << endl;
    return false;
  }

  cout << "OK" << endl;
  return true;
}

string manualTest[7] = {"aaaa aaa aaaaaaaaaaaaaa a aaaaaaa a a aaaaaa aaa  a aa a aaaaaaa aa aaa ",
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                        "                                ",
                        "                              ba",
                        "bfffffffffffffffffffffffffffffa ",
                        " ba ",
                        "abacaba"};

bool manualTesting() {
  size_t sz = sizeof(manualTest) / sizeof(string);
  for (size_t i = 0; i < sz; ++i) {
    if (!test(manualTest[i].c_str(), manualTest[i].size(), "manual testing")) {
      return false;
    }
  }
  return true;
}

void genRandomString(char* str, size_t len, size_t number_of_spaces) {
  assert(len >= number_of_spaces);
  vector<int> poses(len);
  for (size_t i = 0; i < len; ++i) {
    poses[i] = i;
    char c = 'a' + (rand() % ('z' - 'a') + 1);
    str[i] = c;
  }
  random_shuffle(poses.begin(), poses.end());
  for (size_t i = 0; i < number_of_spaces; ++i) {
    str[poses[i]] = ' ';
  }
}

bool shiftedString(size_t len, size_t number_of_spaces, size_t prev_shifts, size_t post_shifts) {
  unique_ptr<char[]> str(new char[len + post_shifts]);
  genRandomString(str.get(), len + post_shifts, number_of_spaces);

  for (size_t i = 0; i < prev_shifts; ++i) {
    for (size_t j = 0; j < post_shifts; ++j) {
      string tag = "shifted string (";
      tag += to_string(i);
      tag += ", ";
      tag += to_string(j);
      tag += ")";

      if (!test(str.get() + i, len + j - i, tag)) {
        return false;
      }
    }
  }
  return true;
}

bool shiftedTests() {
  if (!shiftedString(10, 4, 4, 4)) {
    return false;
  }
  cout << endl;

  if (!shiftedString(30, 15, 5, 5)) {
    return false;
  }
  cout << endl;

  if (!shiftedString(70, 20, 20, 20)) {
    return false;
  }
  cout << endl;

  if (!shiftedString(1000, 300, 17, 17)) {
    return false;
  }
  cout << endl;

  if (!shiftedString(1000000, 100000, 20, 20)) {
    return false;
  }
  cout << endl;

  if (!shiftedString(1000000, 1000000, 20, 20)) {
    return false;
  }
  cout << endl;

  return true;
}

int main() {
  srand(12);
  if (!manualTesting()) {
    return 0;
  }
  cout << endl;

  if (!shiftedTests()) {
    return 0;
  }

  return 0;
}
