#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point {
  int x;
  int y;
};

struct line {
  struct point p1;
  struct point p2;
};

size_t get_n_inputs(char *input) {
  size_t n_inputs;

  n_inputs = 1;
  while (*input) {
    if (*input == ',') ++n_inputs;
    ++input;
  }
  return n_inputs;
}

void split_string(char *input, char **inputs) {
  char *tok;

  tok = strtok(input, ",");
  do {
    *inputs++ = tok;
  } while ((tok = strtok(NULL, ",")));
}

int get_input_list(char *input, size_t *out_count, char ***out_inputs) {
  *out_count = get_n_inputs(input);
  *out_inputs = malloc(*out_count * sizeof(char *));
  if (!*out_inputs) return -1;
  split_string(input, *out_inputs);
  return 0;
}

int calc_line_segs(
  size_t n_inputs,
  char **inputs,
  size_t *out_linecount,
  struct line **out_lines
) {
  struct point p = { 0 };
  struct line prev_line = { 0 }, next_line, *out_line;

  *out_linecount = n_inputs - 1;
  *out_lines = malloc(*out_linecount * sizeof(struct line));
  if (!*out_lines) return -1;
  out_line = *out_lines;
  while (--n_inputs) {
    int run;

    /* Add 1 to inputs to ignore the leading direction character (R, U, D, L) */
    if (sscanf((*inputs) + 1, "%d", &run) == EOF) return -1;
    next_line = prev_line;
    switch ((*inputs)[0]) {
    case 'R': p.x += run; break;
    case 'U': p.y += run; break;
    case 'L': p.x -= run; break;
    case 'D': p.y -= run; break;
    }
    next_line.p1 = prev_line.p2;
    next_line.p2 = p;
    prev_line = next_line;
    *out_line++ = next_line;
    ++inputs;
  }
  return 0;
}

int main(int arg, char **argv) {
  char **first_inputs = NULL, **second_inputs = NULL;
  char first[] = "R75,D30,R83,U83,L12,D49,R71,U7,L72";
  char second[] = "U62,R66,U55,R34,D71,R55,D58,R83";
  size_t i, n_first, n_second, n_first_lines, n_second_lines;
  struct line *first_lines = NULL, *second_lines = NULL;

  if (get_input_list(first, &n_first, &first_inputs)) return -1;
  if (get_input_list(second, &n_second, &second_inputs)) return -1;
  if (calc_line_segs(
        n_first,
        first_inputs,
        &n_first_lines,
        &first_lines
      )) {
    return -1;
  }
  if (calc_line_segs(
        n_second,
        second_inputs,
        &n_second_lines,
        &second_lines
      )) {
    return -1;
  }
  puts("first inputs");
  for (i = 0; i < n_first; ++i) {
    printf("%4s\n", first_inputs[i]);
  }
  puts("second inputs");
  for (i = 0; i < n_second; ++i) {
    printf("%4s\n", second_inputs[i]);
  }
  puts("first lines");
  for (i = 0; i < n_first_lines; ++i) {
    printf(
      "  ( % 4d % 4d ) ( % 4d % 4d )\n",
      first_lines[i].p1.x,
      first_lines[i].p1.y,
      first_lines[i].p2.x,
      first_lines[i].p2.y
    );
  }
  puts("second lines");
  for (i = 0; i < n_second_lines; ++i) {
    printf(
      "  ( % 4d % 4d ) ( % 4d % 4d )\n",
      second_lines[i].p1.x,
      second_lines[i].p1.y,
      second_lines[i].p2.x,
      second_lines[i].p2.y
    );
  }
  free(first_lines);
  free(second_lines);
  free(first_inputs);
  free(second_inputs);
  return 0;
}
