#include <ruby.h>
// TODO: debug
#include <stdio.h>

#include "heatshrink_encoder.h"
#include "heatshrink_decoder.h"

#define INPUT_BUFFER_SIZE 256

struct Buffer {
  uint8_t *p;
  size_t n;
};

// sink data from src buffer into xcoder
#define HSRUBY_SINK(K, kind, src, hsd)                                         \
  do {                                                                         \
    size_t nread;                                                              \
    HS##K##_sink_res sres =                                                    \
        heatshrink_##kind##_sink(hsx, src.p, src.n, &nread);                   \
    if (sres < 0)                                                              \
      rb_raise(rb_eRuntimeError, "Failed to sink data " #kind " %d.", sres);   \
    src.p += nread;                                                            \
    src.n -= nread;                                                            \
  } while (0);

// poll data from xcoder and append it to ruby string
#define HSRUBY_POLL_CAT(K, kind, hsx, str)                                     \
  HS##K##_poll_res pres;                                                       \
  do {                                                                         \
    size_t nread;                                                              \
    uint8_t buf[1024];                                                         \
    pres = heatshrink_##kind##_poll(hsx, buf, sizeof(buf), &nread);            \
    if (pres < 0)                                                              \
      rb_raise(rb_eRuntimeError, "Failed to poll data %d.", pres);             \
    rb_str_cat(res, (char *)buf, nread);                                       \
  } while (pres == HS##K##R_POLL_MORE);

// finish xcoder
#define HSRUBY_FINISH(X, xcoder, hsx, res)                                     \
  HS##X##_finish_res fres;                                                     \
  while (1) {                                                                  \
    fres = heatshrink_##xcoder##_finish(hsx);                                  \
    if (fres == HS##X##R_FINISH_DONE)                                          \
      break;                                                                   \
    if (fres < 0)                                                              \
      rb_raise(rb_eRuntimeError, "Failed to finish %d.", fres);                \
    HSRUBY_POLL_CAT(X, xcoder, hsx, res);                                      \
  }

// alloc a decoder
#define HSRUBY_ALLOC_D(ibs, wsz2, lasz2)                                       \
  heatshrink_decoder_alloc(ibs, wsz2, lasz2)
// alloc an encoder
#define HSRUBY_ALLOC_E(ibs, wsz2, lasz2) heatshrink_encoder_alloc(wsz2, lasz2)
// free an xcoder
#define HSRUBY_FREE(X, xcoder, hsx) heatshrink_##xcoder##_free(hsx)

// encode/decode implementation
#define HSRUBY_IMPL(X, xcoder)                                                 \
  Check_Type(data, T_STRING);                                                  \
  Check_Type(window_sz2, T_FIXNUM);                                            \
  Check_Type(lookahead_sz2, T_FIXNUM);                                         \
                                                                               \
  struct Buffer src = {(uint8_t *)RSTRING_PTR(data), RSTRING_LEN(data)};       \
  VALUE res = rb_str_new(NULL, 0);                                             \
                                                                               \
  int wsz2 = FIX2INT(window_sz2);                                              \
  int lasz2 = FIX2INT(lookahead_sz2);                                          \
                                                                               \
  heatshrink_##xcoder *hsx = HSRUBY_ALLOC_##X(INPUT_BUFFER_SIZE, wsz2, lasz2); \
  if (hsx == NULL)                                                             \
    rb_raise(rb_eRuntimeError, "Failed to allocate " #xcoder ".");             \
                                                                               \
  while (src.n) {                                                              \
    HSRUBY_SINK(X, xcoder, src, hsx);                                          \
    HSRUBY_POLL_CAT(X, xcoder, hsx, res);                                      \
  }                                                                            \
  HSRUBY_FINISH(X, xcoder, hsx, res);                                          \
  HSRUBY_FREE(X, xcoder, hsx);                                                 \
  return res;

static VALUE compress(VALUE self, VALUE data, VALUE window_sz2,
                      VALUE lookahead_sz2) {
  HSRUBY_IMPL(E, encoder);
}

static VALUE uncompress(VALUE self, VALUE data, VALUE window_sz2,
                        VALUE lookahead_sz2) {
  HSRUBY_IMPL(D, decoder);
}

void Init_heatshrink(void) {
  VALUE mod = rb_define_module("Heatshrink");
  mod = rb_define_module_under(mod, "Internal");
  rb_define_module_function(mod, "compress", &compress, 3);
  rb_define_module_function(mod, "uncompress", &uncompress, 3);
}
