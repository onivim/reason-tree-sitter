#include <tree_sitter/api.h>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

CAMLprim value rets_hello(value unit) {
  printf("Hello, world!\n");
  return Val_unit;
}
