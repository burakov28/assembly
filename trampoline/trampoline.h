#ifndef TRAMPOLINE_H_
#define TRAMPOLINE_H_

#include <sys/mman.h>

#include <memory>
#include <vector>



class MemoryManager {
 public:
  static void* getPage() {
    if (pages_.empty()) {
      allocatePages();
    }
    void* page = pages_.back();
    pages_.pop_back();
    return page;
  }

  static void returnPage(void* page) {
    pages_.push_back(page);
  }

  static void allocatePages() {
    void* firstPage = mmap(nullptr,
                           PAGE_SIZE * PAGE_NUMBER,
                           PROT_EXEC | PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS,
                           -1,
                           0);
    char* byByte = static_cast<char*>(firstPage);
    for (int i = 0; i < PAGE_NUMBER; ++i) {
      pages_.push_back(byByte);
      byByte += PAGE_SIZE;
    }
  }

 private:
  static std::vector<void*> pages_;

  static const int PAGE_SIZE = 4096;
  static const int PAGE_NUMBER = 16;
};

std::vector<void*> MemoryManager::pages_;

template <typename... Args>
class Trampoline {};


template <typename R, typename... Args>
class Trampoline<R(Args...)> : MemoryManager {
 public:
  typedef R(FuncPtr)(Args...);

  template <typename T>
  Trampoline(const T& func) : copier_(copierGen<T>),
                              deleter_(deleterGen<T>),
                              funcPtr_(new T(func)),
                              caller_(call<T>),
                              code_(generateCode(getPage())) {}

  Trampoline(const Trampoline& other) : copier_(other.copier_),
                                        deleter_(other.deleter_),
                                        funcPtr_(copier_(other.funcPtr_)),
                                        caller_(other.caller_),
                                        code_(generateCode(getPage())) {}

  ~Trampoline() {
    deleter_(funcPtr_);
    returnPage(code_);
  }



  FuncPtr* get() const {
    return reinterpret_cast<R(*)(Args...)>(code_);
  }

 private:
  struct __attribute__((packed)) NotAlignedStorage {
    char c;
    void* funcPtr;
    void* emptyWord;
  };

  template <typename T>
  static void deleterGen(void* ptr) {
    delete static_cast<T*>(ptr);
  }

  template <typename T>
  static void* copierGen(void* funcPtr) {
    return new T(*static_cast<T*>(funcPtr));
  }

  void* generateCode(void* ptr) {
    char* bytePtr = static_cast<char*>(ptr);

    // 0x4883EC10 sub rsp, 16
    *bytePtr++ = 0x48;
    *bytePtr++ = 0x83;
    *bytePtr++ = 0xEC;
    *bytePtr++ = 0x10;

    // 0x49BB mov r11 imm
    *bytePtr++ = 0x49;
    *bytePtr++ = 0xBB;

    // place object address to prev command
    *((void**)bytePtr) = funcPtr_;
    bytePtr += 8;

    // 0x4C895C2401 mov [rsp + 1] r11
    *bytePtr++ = 0x4C;
    *bytePtr++ = 0x89;
    *bytePtr++ = 0x5C;
    *bytePtr++ = 0x24;
    *bytePtr++ = 0x01;

    // 0x49BB mov r11 imm
    *bytePtr++ = 0x49;
    *bytePtr++ = 0xBB;

    // place caller address to prev command
    *((void**)bytePtr) = (void*) caller_;
    bytePtr += 8;

    // 0x41FFD3 call r11
    *bytePtr++ = 0x41;
    *bytePtr++ = 0xFF;
    *bytePtr++ = 0xD3;

    // 0x4883C410 add rsp 16
    *bytePtr++ = 0x48;
    *bytePtr++ = 0x83;
    *bytePtr++ = 0xC4;
    *bytePtr++ = 0x10;

    // 0xC3 ret
    *bytePtr++ = 0xC3;

    return ptr;
  }

  template <typename T>
  static R call(NotAlignedStorage storage, Args... args) {
    return (*static_cast<T*>(storage.funcPtr))(std::forward<Args>(args)...);
  }

  void* (*copier_)(void*);
  void (*deleter_)(void*);
  void* funcPtr_;
  R (*caller_)(NotAlignedStorage, Args...);
  void* code_;
};

#endif  // TRAMPOLINE_H_
