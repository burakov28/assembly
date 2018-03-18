#ifndef WORDS_WORDS_H_
#define WORDS_WORDS_H_

#include <cstring>

extern "C" int wordsNasm(const char*, size_t);

namespace nasm {

int words(const char* src, size_t len) {
  if (len < 16) {
    if (len == 0) {
      return 0;
    }
    int ans = 0;
    bool is_prev_space = true;
    for (size_t i = 0; i < len; ++i) {
      if (src[i] == ' ') {
        is_prev_space = true;
      } else {
        if (is_prev_space) {
          ++ans;
        }
        is_prev_space = false;
      }
    }
    return ans;
  } else {
    return wordsNasm(src, len);
  }
}

}  // namespace nasm

#endif  // WORDS_WORDS_H_
