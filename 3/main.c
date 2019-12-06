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

int intersection(struct line *a, struct line *b, struct point *out_p) {
  if (!a) return 0;
  if (!b) return 0;
  /* Check if line a is horizontal or vertical */
  if ((a->p1.y - a->p2.y) == 0) {
    /* Here, a is horizontal */
    if ((b->p1.y - b->p2.y) == 0) return 0;
    if ((a->p1.y >= b->p2.y) && (a->p1.y <= b->p1.y)) {
      if ((b->p1.x >= a->p1.x) && (b->p1.x <= a->p2.x)) {
        if (!out_p) return 1;
        out_p->x = b->p1.x;
        out_p->y = a->p1.y;
        return 1;
      }
    }
  } else {
    /* Here, a is vertical */
    if ((b->p1.x - b->p2.x) == 0) return 0;
    if ((a->p1.x >= b->p2.x) && (a->p1.x <= b->p1.x)) {
      if ((b->p1.y >= a->p1.y) && (b->p1.y <= a->p2.y)) {
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

int main(int arg, char **argv) {
  char **first_inputs = NULL, **second_inputs = NULL;
  char first[] = "R75,D30,R83,U83,L12,D49,R71,U7,L72";
  char second[] = "U62,R66,U55,R34,D71,R55,D58,R83";
  size_t i, n_first, n_second, n_first_lines, n_second_lines, n_intersections;
  struct line *first_lines = NULL, *second_lines = NULL;
  struct point *intersections;

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
  for (i = 0; i < n_intersections; ++i) {
    printf("( % 4d % 4d )\n", intersections[i].x, intersections[i].y);
  }
  free(intersections);
  free(first_lines);
  free(second_lines);
  free(first_inputs);
  free(second_inputs);
  return 0;
}
