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
  state->board =
      malloc(sizeof(char) * ((state->x_size + 1) * (state->y_size + 1)));
  state->board[0] = "##############";
  state->board[1] = "#            #";
  state->board[2] = "#        *   #";
  state->board[3] = "#            #";
  state->board[4] = "#   d>       #";
  state->board[5] = "#            #";
  state->board[6] = "#            #";
  state->board[7] = "#            #";
  state->board[8] = "#            #";
  state->board[9] = "##############";
  state->snakes = malloc(sizeof(snake_t) * state->num_snakes);
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
    for (size_t j = 0; j < state->x_size; j++) {
      fprintf(fp, "%s", &state->board[i][j]);
    }
    fprintf(fp, "\n");
  }
}
/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  return c == 'T';
  return true;
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  return c == 'H' || c == 'T';
  return true;
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  return 'T';
  return '?';
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if (c == 'E') {
    return 1;
  } else if (c == 'W') {
    return -1;
  } else {
    return 0;
  }
  return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  if (c == 'N') {
    return -1;
  } else if (c == 'S') {
    return 1;
  } else {
    return 0;
  }
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;
  return '?';
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;

  return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;

  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  char next = next_square(state, 0);
  if (is_snake(next)) {
    state->snakes->live = false;
  } else if (is_tail(next)) {
    state->snakes->live = false;
  } else if (next == '.') {
    update_head(state, 0);
    update_tail(state, 0);
  } else if (next == 'F') {
    update_head(state, 0);
    add_food(state);
  }
  return;
}

/* Task 5 */
game_state_t *load_board(char *filename) {
  // TODO: Implement this function.
  FILE *f = fopen(filename, "r");
  game_state_t *state = malloc(sizeof(game_state_t));
  state->snakes = malloc(sizeof(snake_t));
  state->snakes->live = true;

  int x = 0;
  int y = 0;
  int c;
  return NULL;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;

  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
