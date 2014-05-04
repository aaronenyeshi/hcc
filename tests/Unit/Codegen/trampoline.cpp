// RUN: %amp_device  -c -S -emit-llvm %s -o -|%cppfilt|%FileCheck %s
#include "amp.h"
class baz {
 public:
  void operator()(Concurrency::index<1> idx) restrict(amp) {
  }
#if 0
  // The declaration and body of this function will be generated
  static __attribute__((annotate("__cxxamp_trampoline")))
  void __cxxamp_trampoline(int foo, float bar) restrict(amp);
#endif
 private:
  int foo;
  float bar;
};
template<typename Foo>
void kerker(void) restrict(amp) {
  // This reference triggers declaration&definition of __cxxamp_trampoline
  int* b = reinterpret_cast<int*>(&Foo::__cxxamp_trampoline);
}
void kk(void) restrict(amp) {
  kerker<baz>();
}
// The definition should be generated by clang
// CHECK: define {{.*}}void @baz::__cxxamp_trampoline
// and also metadata
// CHECK: !0 = metadata !{void (i32, float)* @baz::__cxxamp_trampoline(int, float)
