#include "state.h"
#include "snake_utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);
static void set_board_at(game_state_t *state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t *state, int snum);
static char next_square(game_state_t *state, int snum);
static void update_tail(game_state_t *state, int snum);
static void update_head(game_state_t *state, int snum);

/* Helper function to get a character from the board (already implemented for
 * you). */
static char get_board_at(game_state_t *state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for
 * you). */
static void set_board_at(game_state_t *state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state() {
  // TODO: Implement this function.
  game_state_t *state = malloc(sizeof(game_state_t));
  // The board has 10 rows and 14 columns.
  state->x_size = 14;
  state->y_size = 10;
  state->num_snakes = 1;
  // https://stackoverflow.com/questions/2614249/dynamic-memory-for-2d-char-array
  state->board = calloc(state->x_size, sizeof(char *));
  for (int i = 0; i < state->x_size; i++) {
    state->board[i] = calloc(state->y_size, sizeof(char));
  }
  strcpy(state->board[0], "##############");
  strcpy(state->board[1], "#            #");
  strcpy(state->board[2], "#        *   #");
  strcpy(state->board[3], "#            #");
  strcpy(state->board[4], "#   d>       #");
  strcpy(state->board[5], "#            #");
  strcpy(state->board[6], "#            #");
  strcpy(state->board[7], "#            #");
  strcpy(state->board[8], "#            #");
  strcpy(state->board[9], "##############");
  state->snakes = calloc(state->num_snakes, sizeof(snake_t));
  // The tail is at row 4, column 4, and the head is at row 4, column 5.
  state->snakes[0].head_x = 5;
  state->snakes[0].head_y = 4;
  state->snakes[0].tail_x = 4;
  state->snakes[0].tail_y = 4;
  state->snakes[0].live = true;
  return state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  // TODO: Implement this function.
  free(state->snakes);
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  // TODO: Implement this function.
  for (size_t i = 0; i < state->y_size; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }
}
/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
// bool is_tail(char c): Returns true if c is part of the snake's tail. The
// snake's tail consists of these characters: wasd. Returns false otherwise.
static bool is_tail(char c) {
  // TODO: Implement this function.
  return c == 'w' || c == 'a' || c == 's' || c == 'd';
}
// bool is_snake(char c): Returns true if c is part of the snake. The snake
// consists of these characters: wasd^<>vx. Returns false otherwise.
static bool is_snake(char c) {
  // TODO: Implement this function.
  char s[] = "wasd^<>vx";
  for (size_t i = 0; i < strlen(s); i++) {
    if (c == s[i]) {
      return true;
    }
  }
  return false;
}
// char body_to_tail(char c): Converts a character in the snake's body (^<>v) to
// the matching character representing the snake's tail (wasd).
static char body_to_tail(char c) {
  // TODO: Implement this function.
  if (c == '^') {
    return 'w';
  } else if (c == '<') {
    return 'a';
  } else if (c == 'v') {
    return 's';
  } else if (c == '>') {
    return 'd';
  }
  return c;
}
// int incr_x(char c): Returns 1 if c is > or d. Returns -1 if c is < or a.
// Returns 0 otherwise.
static int incr_x(char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd') {
    return 1;
  } else if (c == '<' || c == 'a') {
    return -1;
  }
  return 0;
}
// int incr_y(char c): Returns 1 if c is v or s. Returns -1 if c is ^ or w.
// Returns 0 otherwise.
static int incr_y(char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's') {
    return 1;
  } else if (c == '^' || c == 'w') {
    return -1;
  }
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].head_x;
  int y = state->snakes[snum].head_y;
  int dx = incr_x(get_board_at(state, x, y));
  int dy = incr_y(get_board_at(state, x, y));
  return get_board_at(state, x + dx, y + dy);
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].head_x;
  int y = state->snakes[snum].head_y;
  int dx = incr_x(get_board_at(state, x, y));
  int dy = incr_y(get_board_at(state, x, y));
  set_board_at(state, x + dx, y + dy, get_board_at(state, x, y));
  state->snakes[snum].head_x = x + dx;
  state->snakes[snum].head_y = y + dy;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].tail_x;
  int y = state->snakes[snum].tail_y;
  char old_tail = get_board_at(state, x, y);
  int new_tail_x = x + incr_x(old_tail);
  int new_tail_y = y + incr_y(old_tail);
  char new_tail = get_board_at(state, new_tail_x, new_tail_y);
  set_board_at(state, new_tail_x, new_tail_y, body_to_tail(new_tail));
  set_board_at(state, x, y, ' ');
  state->snakes[snum].tail_x = new_tail_x;
  state->snakes[snum].tail_y = new_tail_y;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  char const next = next_square(state, 0);
  if (is_snake(next) || next == '#') {
    state->snakes->live = false;
    set_board_at(state, state->snakes->head_x, state->snakes->head_y, 'x');
  } else if (next == ' ') {
    update_head(state, 0);
    update_tail(state, 0);
  } else if (next == '*') {
    update_head(state, 0);
    add_food(state);
  }
  return;
}

/* Task 5 */
game_state_t *load_board(char *filename) {
  // TODO: Implement this function.
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    return NULL;
  }
  game_state_t *state = create_default_state();
  int i = 0;
  while (fgets(state->board[i], 100, f) != NULL) {
    state->board[i][strlen(state->board[i]) - 1] = '\0';
    i++;
  }
  state->x_size = strlen(state->board[0]);
  state->y_size = i;
  fclose(f);
  return state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  int tail_x = state->snakes[snum].tail_x;
  int tail_y = state->snakes[snum].tail_y;
  char tail = get_board_at(state, tail_x, tail_y);
  while (incr_x(tail) != 0 || incr_y(tail) != 0) {
    tail_x += incr_x(tail);
    tail_y += incr_y(tail);
    tail = get_board_at(state, tail_x, tail_y);
  }
  state->snakes[snum].head_x = tail_x;
  state->snakes[snum].head_y = tail_y + 1; // it seems that this may work...
}

/* Task 6.2 */

static void find_head_dfs(game_state_t *state, int *snum,
                          bool visited[100][100], int x, int y) {
  if (x < 0 || x >= state->x_size || y < 0 || y >= state->y_size) {
    return;
  }
  visited[x][y] = true;
  char head = get_board_at(state, x, y);
  while (incr_x(head) != 0 || incr_y(head) != 0) {
    x += incr_x(head);
    y += incr_y(head);
    head = get_board_at(state, x, y);
    visited[x][y] = true;
  }
  state->snakes[*snum].head_x = x;
  state->snakes[*snum].head_y = y + 1; // it seems that this may work...
  (*snum)++;
  state->num_snakes = *snum;
}
// your need to iter 4 directions to find the tail.
static void find_tail_dfs(game_state_t *state, int *snum,
                          bool visited[100][100], int x, int y) {
  if (x < 0 || x >= state->x_size || y < 0 || y >= state->y_size) {
    return;
  }
  visited[x][y] = true;
  char tail = get_board_at(state, x, y);
  while (incr_x(tail) != 0 || incr_y(tail) != 0) {
    x -= incr_x(tail);
    y -= incr_y(tail);
    tail = get_board_at(state, x, y);
    visited[x][y] = true;
  }
  state->snakes[*snum].head_x = x;
  state->snakes[*snum].head_y = y; // it seems that this may work...
}

// whenever we find a snake, we need to find its head and tail.
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  int snum = 0;
  bool visited[100][100] = {false};
  for (int i = 0; i < state->y_size; i++) {
    for (int j = 0; j < state->x_size; j++) {
      if (is_snake(get_board_at(state, j, i)) && !visited[i][j]) {
        printf("%d  %d\n", j, i);
        find_head_dfs(state, &snum, visited, j, i);
        find_tail_dfs(state, &snum, visited, j, i);
      }
    }
  }
  return state;
}
