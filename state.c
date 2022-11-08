#include "state.h"
#include "snake_utils.h"
#include <ctype.h>
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
  char const c = get_board_at(state, x + dx, y + dy);
  return c;
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
  set_board_at(state, new_tail_x, new_tail_y,
               body_to_tail(get_board_at(state, new_tail_x, new_tail_y)));
  set_board_at(state, x, y, ' ');
  state->snakes[snum].tail_x = new_tail_x;
  state->snakes[snum].tail_y = new_tail_y;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  for (size_t snum = 0; snum < state->num_snakes; snum++) {
    char const next = next_square(state, snum);
    if (is_snake(next) || next == '#') {
      state->snakes[snum].live = false;
      set_board_at(state, state->snakes[snum].head_x,
                   state->snakes[snum].head_y, 'x');
    } else if (isspace(next)) {
      update_head(state, snum);
      update_tail(state, snum);
    } else if (next == '*') {
      update_head(state, snum);
      add_food(state);
    }
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
  state->board = realloc(state->board, 100 * sizeof(char *));
  for (size_t i = 0; i < 100; i++) {
    state->board[i] = malloc(200 * sizeof(char));
  }
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
// https://github.com/Jonaschann/shop/blob/2a7c51735c3e9b97debe94d9652e931533117128/sp22-proj1/state.c
static void find_head(game_state_t *state, int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;
  int x = snake->tail_x, y = snake->tail_y;
  int prevx, prevy;
  char c = get_board_at(state, snake->tail_x, snake->tail_y);
  while (is_snake(c)) {
    prevx = x;
    prevy = y;
    x += incr_x(c);
    y += incr_y(c);
    // snake is dead
    if (x == prevx && y == prevy) {
      break;
    }
    c = get_board_at(state, x, y);
  }
  snake->head_x = prevx;
  snake->head_y = prevy;
}
/* Task 6.2 */

game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  state->num_snakes = 0;
  // count the number of snakes
  for (int i = 0; i < state->y_size; i++) {
    for (int j = 0; j < state->x_size; j++) {
      if (is_tail(get_board_at(state, j, i))) {
        state->num_snakes++;
      }
    }
  }
  state->snakes = realloc(state->snakes, state->num_snakes * sizeof(snake_t));
  int snum = 0;
  for (size_t x = 0; x < state->x_size; x++) {
    for (size_t y = 0; y < state->y_size; y++) {
      if (is_tail(get_board_at(state, x, y))) {
        state->snakes[snum].tail_x = x;
        state->snakes[snum].tail_y = y;
        find_head(state, snum);
        state->snakes[snum].live = true;
        snum++;
      }
    }
  }
  return state;
}