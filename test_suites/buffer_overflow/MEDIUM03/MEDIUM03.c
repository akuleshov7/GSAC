/*
 * Based on CVE-2023-0819
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Info {
  unsigned char *data;
  int size;
};

typedef struct {
  unsigned short year;
  unsigned char month;
  unsigned char day;
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} TIME;

void foo(struct Info info) {
  unsigned char *data =
      info.data; // 'data' is an array with unsigned char elements. With 'info.size' length.
  int data_size = info.size;
  TIME time_table;

  unsigned int date, yp, mp, k;

  if (data_size != 5) {
    printf("Corrupted size\n");
  }

  date = data[0] * 256 + data[1];
  yp = (unsigned int)((date - 15078.2) / 365.25);
  mp = (unsigned int)((date - 14956.1 - (unsigned int)(yp * 365.25)) / 30.6001);
  time_table.day =
      (unsigned int)(date - 14956 - (unsigned int)(yp * 365.25) - (unsigned int)(mp * 30.6001));
  if (mp == 14 || mp == 15)
    k = 1;
  else
    k = 0;
  time_table.year = yp + k + 1900;
  time_table.month = mp - 1 - k * 12;

  time_table.hour = 10 * ((data[2] & 0xf0) >> 4) + (data[2] & 0x0f);
  time_table.minute = 10 * ((data[3] & 0xf0) >> 4) + (data[3] & 0x0f);
  // Access to 'data' is out of bounds
  time_table.second = 10 * ((data[4] & 0xf0) >> 4) + (data[4] & 0x0f); // buffer-overflow
}

int main() {
  struct Info info;
  info.data = malloc(4); // Memory allocation
  memcpy(info.data, "asdf", 4);
  info.size = 4;

  foo(info);

  free(info.data);
}
