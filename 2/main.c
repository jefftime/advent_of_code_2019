#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int PROGRAM[] = {
  1   , 0   , 0   , 3   , 1   , 1   , 2   , 3   , 1  , 3   ,
  4   , 3   , 1   , 5   , 0   , 3   , 2   , 1   , 10 , 19  ,
  1   , 6   , 19  , 23  , 1   , 13  , 23  , 27  , 1  , 6   ,
  27  , 31  , 1   , 31  , 10  , 35  , 1   , 35  , 6  , 39  ,
  1   , 39  , 13  , 43  , 2   , 10  , 43  , 47  , 1  , 47  ,
  6   , 51  , 2   , 6   , 51  , 55  , 1   , 5   , 55 , 59  ,
  2   , 13  , 59  , 63  , 2   , 63  , 9   , 67  , 1  , 5   ,
  67  , 71  , 2   , 13  , 71  , 75  , 1   , 75  , 5  , 79  ,
  1   , 10  , 79  , 83  , 2   , 6   , 83  , 87  , 2  , 13  ,
  87  , 91  , 1   , 9   , 91  , 95  , 1   , 9   , 95 , 99  ,
  2   , 99  , 9   , 103 , 1   , 5   , 103 , 107 , 2  , 9   ,
  107 , 111 , 1   , 5   , 111 , 115 , 1   , 115 , 2  , 119 ,
  1   , 9   , 119 , 0   , 99  , 2   , 0   , 14  , 0
};

int step(int *program, size_t ip) {
  int a, b;
  size_t dst;

  switch (program[ip]) {
  case 1:
    a = program[program[ip + 1]];
    b = program[program[ip + 2]];
    dst = (size_t) program[ip + 3];
    program[dst] = a + b;
    return 1;
  case 2:
    a = program[program[ip + 1]];
    b = program[program[ip + 2]];
    dst = (size_t) program[ip + 3];
    program[dst] = a * b;
    return 1;
  case 99:
    return 0;
  default:
    puts("program error");
    return 0;
  }
  return -1;
}

int main(int argc, char **argv) {
  enum {
    RANGE = 100
  };

  int *prog, i, j;
  size_t ip;

  prog = malloc(sizeof(PROGRAM));
  if (!prog) return -1;
  for (i = 0; i < RANGE; ++i) {
    for (j = 0; j < RANGE; ++j) {
      ip = 0;
      memcpy(prog, PROGRAM, sizeof(PROGRAM));
      prog[1] = i;
      prog[2] = j;
      while (step(prog, ip)) ip += 4;
      if (prog[0] == 19690720) goto done;
    }
  }
  puts(":(");
  free(prog);
  return -1;

 done:
  printf("noun: %d, verb: %d, value: %d\n", i, j, prog[0]);
  free(prog);
  return 0;
}
