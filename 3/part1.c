#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct point {
  int x;
  int y;
};

struct line {
  struct point p1;
  struct point p2;
};

char *read_input(char *filename, size_t *out_size) {
  char *output;
  FILE *f;
  size_t size;

  if (!filename) return NULL;
  f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  size = (size_t) ftell(f);
  fseek(f, 0, SEEK_SET);
  output = malloc(size);
  if (!output) return NULL;
  fread(output, 1, size, f);
  fclose(f);
  *out_size = size;
  return output;
}

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

void set_limits(int a, int b, int *out_min, int *out_max) {
  if (!out_max) return;
  if (!out_min) return;
  if (a < b) {
    *out_min = a;
    *out_max = b;
  } else {
    *out_min = b;
    *out_max = a;
  }
}

int intersection(struct line *a, struct line *b, struct point *out_p) {
  int xmax, xmin, ymax, ymin;

  if (!a) return 0;
  if (!b) return 0;
  /* Check if line a is horizontal or vertical */
  if ((a->p1.y - a->p2.y) == 0) {
    /* Here, a is horizontal */
    if ((b->p1.y - b->p2.y) == 0) return 0;
    set_limits(a->p1.x, a->p2.x, &xmin, &xmax);
    set_limits(b->p1.y, b->p2.y, &ymin, &ymax);
    if (a->p1.y >= ymin && a->p1.y <= ymax) {
      if (b->p1.x >= xmin && b->p2.x <= xmax) {
        if (!out_p) return 1;
        out_p->x = b->p1.x;
        out_p->y = a->p1.y;
        return 1;
      }
    }
  } else {
    /* Here, a is vertical */
    if ((b->p1.x - b->p2.x) == 0) return 0;
    set_limits(a->p1.y, a->p2.y, &ymin, &ymax);
    set_limits(b->p1.x, b->p2.x, &xmin, &xmax);
    if (a->p1.x >= xmin && a->p1.x <= xmax) {
      if (b->p1.y >= ymin && b->p1.y <= ymax) {
        if (!out_p) return 1;
        out_p->x = a->p1.x;
        out_p->y = b->p1.y;
        return 1;
      }
    }
  }
  return 0;
}

int calc_intersections(
  size_t n_first,
  struct line *first_lines,
  size_t n_second,
  struct line *second_lines,
  size_t *out_count,
  struct point **out_intersections
) {
  size_t i, j;
  struct point p, *out_intersection;

  *out_intersections = malloc(n_first * n_second * sizeof(struct point));
  if (!*out_intersections) return -1;
  out_intersection = *out_intersections;
  for (i = 0; i < n_first; ++i) {
    for (j = 0; j < n_second; ++j) {
      if (intersection(first_lines + i, second_lines + j, &p)) {
        *out_intersection++ = p;
        ++(*out_count);
      }
    }
  }
  return 0;
}

void print_closest_intersection(
  size_t n_intersections,
  struct point *intersections
) {
  struct point min = { INT_MAX / 2, INT_MAX / 2 };

  if (!n_intersections) return;
  if (!intersections) return;
  while (--n_intersections) {
    struct point cur;

    cur = *intersections++;
    if ((abs(cur.x) + abs(cur.y)) < (abs(min.x) + abs(min.y))) {
      if (abs(cur.x) + abs(cur.y) > 0) min = cur;
    }
  }
  printf("( % 4d % 4d ) -> %u\n", min.x, min.y, abs(min.x) + abs(min.y));
}

int main(int arg, char **argv) {
  char **first_inputs = NULL, **second_inputs = NULL;
  char *first, *second;
  size_t first_size, second_size, n_first, n_second;
  size_t n_first_lines, n_second_lines, n_intersections = 0;
  struct line *first_lines = NULL, *second_lines = NULL;
  struct point *intersections;

  first = read_input("../3/first.txt", &first_size);
  second = read_input("../3/second.txt", &second_size);
  if (!first) return -1;
  if (!second) return -1;
  if (get_input_list(first, &n_first, &first_inputs)) return -1;
  if (get_input_list(second, &n_second, &second_inputs)) return -1;
  /* Remove \n at the end */
  first_size -= 1;
  second_size -= 1;
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
  if (calc_intersections(
        n_first_lines,
        first_lines,
        n_second_lines,
        second_lines,
        &n_intersections,
        &intersections
      )) {
    return -1;
  }
  print_closest_intersection(n_intersections, intersections);
  free(intersections);
  free(first_lines);
  free(second_lines);
  free(first_inputs);
  free(second_inputs);
  free(first);
  free(second);
  return 0;
}
