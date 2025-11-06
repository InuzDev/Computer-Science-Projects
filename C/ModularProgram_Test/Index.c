#include <stdio.h>
// Como hacer un programa modular en c

void HelloWorld(char String[])
{
  printf("%s", String);
}

int main()
{
  char String[] = "Hello, World\n";
  HelloWorld(String);
  // printf("\n%s", StringTestPut);
  return 0;
}
