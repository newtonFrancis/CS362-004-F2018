#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // generate random characters from ASCII 32 - 126
    int n = rand() % (126 - 32 + 1) + 32;

    // return the random character
    return n;
}

char *inputString()
{
    static char string[5];

    // initialize the string to null terminators
    memset(&string, 0, sizeof(string));

    // loop through each of the five characters in the string
    for (int i = 0; i < 5; i++)
    {
      // fill each character in the string with a random lower-case letter
      string[i] = (rand() % 26) + 97;

      //string[i] = inputChar();
    }

    // return the string
    return string;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;

  //clock_t begin = clock();

  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' ' && state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;

    if (s[0] == 'r' && s[1] == 'e' &&
        s[2] == 's' && s[3] == 'e' &&
        s[4] == 't' && s[5] == '\0'&&
        state == 9)
    {
      printf("error ");

      //clock_t end = clock();
      //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      //printf("\nTime: %f seconds\n", time_spent);

      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();

    return 0;
}
