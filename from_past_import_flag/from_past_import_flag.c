#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

extern char tables[16*128];

char *decode_buf;

void decode_b()
{
  for (int i = 0; i < 128; i++) {
    decode_buf[i] = 0;
  }
  for (int i = 0; i < 128 * 16; i++) {
    decode_buf[i % 128] ^= tables[i];
  }
}

void decode_a() {
  for (int i = 0; i < 128; i++) {
    decode_buf[i] = 0;
  }
  for (int i = 0; i < 128 * 16; i++) {
    decode_buf[i % 128] -= tables[i];
  }
}

time_t fetch_time_from_ntp();

#define clamp_0_31(x) (x < 0 ? 0 : (x > 31) ? 31 : x)

typedef void (*voidfun)();

int main(int argc, char **argv)
{
  time_t time1 = time(NULL);
  time_t time2 = fetch_time_from_ntp();

  struct tm *tm1 = gmtime(&time1);
  struct tm *tm2 = gmtime(&time2);
  int year1 = tm1->tm_year - 100;
  int year2 = tm2->tm_year - 100;
  year1 = clamp_0_31(year1);
  year2 = clamp_0_31(year2);

  voidfun decode[1024];

  for(int i=0; i<1024; i++) {
    decode[i] = (i == 5*32+5) ? decode_b : decode_a;
  }

  printf("Warning: This program only works if run in 2005\n");

  // printf("Time: %s", ctime((const time_t *)&time1));
  // printf("Time: %s", ctime((const time_t *)&time2));

  decode_buf = malloc(128);
  bzero(decode_buf, 128);
  decode[year1 * 32 + year2]();
  printf("%s\n", decode_buf);
}
