#include<stdio.h>

int main(void)
#define name "Lars Tragl"
#define day 21
#define month 07
#define year 1984

{
  printf("Mein Name ist %s\n", name);
  printf("Mein Geburtsdatum ist der %02d.%02d.%04d\n", day, month, year);
  return 0;
}
