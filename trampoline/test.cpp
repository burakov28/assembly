#include "trampoline.h"

#include <iostream>
#include <set>

using namespace std;

typedef long long LL;

set<LL> func(LL a1, LL a2, LL a3, LL a4, LL a5, LL a6, LL a7, LL a8, LL a9, LL a10, LL a11, LL a12) {

  set<LL> ret;
  LL sum = 0;
  ret.insert(a1), sum += a1;
  ret.insert(a2), sum += a2;
  ret.insert(a3), sum += a3;
  ret.insert(a4), sum += a4;
  ret.insert(a5), sum += a5;
  ret.insert(a6), sum += a6;
  ret.insert(a7), sum += a7;
  ret.insert(a8), sum += a8;
  ret.insert(a9), sum += a9;
  ret.insert(a10), sum += a10;
  ret.insert(a11), sum += a11;
  ret.insert(a12), sum += a12;
  ret.insert(sum);
  return ret;
}

int main() {

  Trampoline<set<LL>(LL, LL, LL, LL, LL, LL, LL, LL, LL, LL, LL, LL)> trampoline([](LL a1, LL a2, LL a3, LL a4, LL a5, LL a6, LL a7, LL a8, LL a9, LL a10, LL a11, LL a12){
    set<LL> ret;
    LL sum = 0;
    ret.insert(a1), sum += a1;
    ret.insert(a2), sum += a2;
    ret.insert(a3), sum += a3;
    ret.insert(a4), sum += a4;
    ret.insert(a5), sum += a5;
    ret.insert(a6), sum += a6;
    ret.insert(a7), sum += a7;
    ret.insert(a8), sum += a8;
    ret.insert(a9), sum += a9;
    ret.insert(a10), sum += a10;
    ret.insert(a11), sum += a11;
    ret.insert(a12), sum += a12;
    ret.insert(sum);
    return ret;
  });

  vector<LL> rnd;
  for (int i = 0; i < 12; ++i) {
    rnd.push_back(rand());
  }

  set<LL> ans1 = func(rnd[0], rnd[1], rnd[2], rnd[3], rnd[4], rnd[5], rnd[6], rnd[7], rnd[8], rnd[9], rnd[10], rnd[11]);

  auto f = trampoline.get();

  set<LL> ans2 = f(rnd[0], rnd[1], rnd[2], rnd[3], rnd[4], rnd[5], rnd[6], rnd[7], rnd[8], rnd[9], rnd[10], rnd[11]);

  if (ans1 == ans2) {
    cout << "Yes" << endl;
  } else {
    cout << "No" << endl;
  }

  return 0;
}
