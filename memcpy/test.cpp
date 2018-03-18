#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <memory>

#include "memcpy.h"

using namespace std;

const size_t MAXLEN = 70;

int timeBuiltIn = 0;
int timeMy = 0;

void genRandomString(char* ptr, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    ptr[i] = 'a' + (rand() % ('z' - 'a' + 1));
  }
}

void print(const char* ptr, size_t len, string tag) {
  cerr << tag << ": ";
  for (size_t i = 0; i < len; ++i) {
    cerr << ptr[i];
  }
  cerr << endl;
}

string normalizeString(const char* ptr, size_t len) {
  string str(ptr, min(len, MAXLEN));
  if (len > MAXLEN) {
    str += "...";
  }

  str += " ";
  str += to_string(len);
  return str;
}

bool test(const char* src, size_t len, string tag, size_t shift = 0) {
  cout << "Testing " << tag << ": " << normalizeString(src, len) << endl;

  unique_ptr<char[]> dstCorrect(new char[len + shift]);
  unique_ptr<char[]> dstTesting(new char[len + shift]);

  int cur = clock();
  memcpy(dstCorrect.get() + shift, src, len);
  int dcur  = clock();
  timeBuiltIn += (dcur - cur);

  cur = clock();
  nasm::memcpy(dstTesting.get() + shift, src, len);
  dcur = clock();
  timeMy += (dcur - cur);

  char* pCorrect = dstCorrect.get() + shift;
  char* pTesting = dstTesting.get() + shift;

  for (size_t i = 0; i < len; ++i) {
    if (pCorrect[i] != pTesting[i]) {
      cerr << "FAIL!" << endl;
      if (len <= MAXLEN) {
        print(pCorrect, len, "Correct");
        print(pTesting, len, "Testing");
        for (size_t j = 0; j < i + 9; ++j) {
          cerr << " ";
        }
        cerr << "^" << endl;
      }
      return false;
    }
  }

  cout << normalizeString(pCorrect, len) << endl;
  cout << normalizeString(pTesting, len) << endl;

  cout << "OK" << endl << endl;
  return true;
}

bool smallTest(size_t len) {
  unique_ptr<char[]> src(new char[len]);
  genRandomString(src.get(), len);

  return test(src.get(), len, "small string");
}

bool testLongRandomString(size_t len) {
  unique_ptr<char[]> src(new char[len]);
  genRandomString(src.get(), len);

  return test(src.get(), len, "long random string");
}

bool testShiftLongString(size_t len, size_t shift_source, size_t shift_dest = 0) {
  unique_ptr<char[]> src(new char[len]);
  char* ptr = src.get();
  genRandomString(ptr, len);
  for (size_t i = 0; i <= shift_source; ++i) {
    for (size_t j = 0; j <= shift_dest; ++j) {
      string tag = "shifted string (";
      tag += to_string(i);
      tag += ", ";
      tag += to_string(j);
      tag += ")";
      if (!test(ptr + i, len - i, tag, j)) {
        return false;
      }
    }
  }

  return true;
}

int main() {
  srand(time(0));

  for (size_t i = 7; i < 32; i += 8) {
    if (!smallTest(i)) {
      return 0;
    }
  }
  cout << endl;

  for (int i = 0; i < 10; ++i) {
    if (!testLongRandomString(50)) {
      return 0;
    }
  }
  cout << endl;

  if (!testShiftLongString(70, 20, 20)) {
    return 0;
  }
  cout << endl;

  for (size_t i = 0, cur = 1; i < 7; ++i, cur *= 10) {
    if (!testLongRandomString(cur)) {
      return 0;
    }
  }
  cout << endl;

  if (!testShiftLongString(10000, 100, 100)) {
    return 0;
  }
  cout << endl;

  cout << "BuiltIn time: " << timeBuiltIn << endl;
  cout << "My time: " << timeMy << endl;
  return 0;
}
