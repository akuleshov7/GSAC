/*
 * Based on CVE-2022-23462 (fixed)
 */

#include <stdio.h>
#include <stdlib.h>

#define IWNUMBUF_SIZE 32

void iwjson_ftoa(long double val, char buf[static IWNUMBUF_SIZE], size_t *out_len) {
  int len = snprintf(buf, IWNUMBUF_SIZE, "%.8Lf", val);
  if (len <= 0) {
    buf[0] = '\0';
    *out_len = 0;
    return;
  }
  while (len > 0 && buf[len - 1] == '0') {
    buf[len - 1] = '\0';
    len--;
  }
  if ((len > 0) && (buf[len - 1] == '.')) {
    buf[len - 1] = '\0';
    len--;
  }
  *out_len = (size_t)len;
}

int main() {
  char buf[IWNUMBUF_SIZE];
  size_t *out_len = malloc(sizeof(size_t));
  *out_len = 50;
  iwjson_ftoa(12345678912345678912345.1234, buf, out_len);
  free(out_len);
}
