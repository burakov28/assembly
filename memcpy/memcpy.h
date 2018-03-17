#ifndef MEMCPY_MEMCPY_H_
#define MEMCPY_MEMCPY_H_

#include <cstring>

extern "C" void memcpyNasm(void* dst, const void* src, size_t len);

namespace nasm {

void memcpy(void* dst, const void* src, size_t len) {
  if (len < 32) {
    char* char_dst = reinterpret_cast<char*>(dst);
    const char* char_src = reinterpret_cast<const char*>(src);

    for (size_t i = 0; i < len; ++i) {
      char_dst[i] = char_src[i];
    }
  } else {
    memcpyNasm(dst, src, len);
  }
}

} // namespace nasm

#endif
