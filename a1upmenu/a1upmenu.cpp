/* Menu program for the arcade1up machines.
 * Functions as a state loop where the only thing that matters is 
 * the selected game index.
 * Moving up (if the selected game index is greater than 1)
 * decrements the index, moving down increments it. 
 * Any time the selected game index changes, 
 * the associated asset (assets/<index>.bgra) is cat'd to
 * the framebuffer /dev/fb0
 * Selecting start shows the help/loading screen (/assets/index.load.bgra)
 * Selecting start after that invokes the associated command
 * and then exists. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SPLASH_DELAY_IN_SECONDS 1
#define SELECT_DELAY_IN_MS 500
#define CONFIRM_DELAY_IN_MS 500
#define MAXIMUM_GAMES 1024

// Note thse should probably be the .so constants.
// Or maybe have a file read that defines the pins?
#define UP     0x01
#define DOWN   0x02
#define SELECT 0x04
#define START  0x08
#define BACK   0x10

enum asset_type {
  selected = 0,
  loading,
  splash,
  max_asset_types,
  invalid_asset_type,
};

int game_count = 0;
int last_selected_game = -1;
int selected_game = 0;
char line_data[1024];

bool get_game_command() {
  FILE* config = NULL;
  int line_counter = 0;
  char *read_value = NULL;
  
  config = fopen("config", "r");
  if (config == NULL) {
    printf("ERROR FINDING CONFIG\r\n");
  }

  do {
      read_value = fgets(line_data, sizeof(line_data)/sizeof(line_data[0]), config);
      if (!read_value) break;
  }
  while (line_counter != selected_game);

  fclose(config);
  return line_counter == selected_game;
}

void show_game_asset(asset_type asset) {
  char asset_buffer[1024];
 
  switch (asset) {
    case selected:
      sprintf(asset_buffer, "cat ./assets/%i.selected.bgra > /dev/fb0", selected_game);
      break;
    case loading:
      sprintf(asset_buffer, "cat ./assets/%i.loading.bgra > /dev/fb0", selected_game);
      break;
    case splash:
      sprintf(asset_buffer, "cat ./assets/splash.bgra > /dev/fb0");
      break;
  }

  // Now cat the file into the framebuffer
  system(asset_buffer);
}

// Read the inputs, and if something changes
// the main loop will redraw.
int read_inputs() {
  // THis function is just responsible for returning a 
  // bit mask of on inputs. The "shared lib" approach
  // seems to return ain int64 masked. Perhaps use that?
  // If not, use the a1upgpio code as a starting point.

  return 0;
}

// Show the game load screen, and if confirmed, start the game
void confirm_and_start_game() {
  struct timespec ts;
  ts.tv_sec = CONFIRM_DELAY_IN_MS / 1000;
  ts.tv_nsec = (CONFIRM_DELAY_IN_MS % 1000) * 1000000;

  show_game_asset(loading);
  sleep(1);

  do {
    int inputs = read_inputs();

    // Allow a cancel.
    if (inputs & BACK) {
      return;
    }

    if (inputs & START) {
      get_game_command();
      execvp(line_data, 0);
      exit(0);
    }

    nanosleep(&ts, &ts);    
  } while(true);
}

int main(void) {
  struct timespec ts;
  ts.tv_sec = SELECT_DELAY_IN_MS / 1000;
  ts.tv_nsec = (SELECT_DELAY_IN_MS % 1000) * 1000000;

  // First find out how many games we have.
  selected_game = MAXIMUM_GAMES;
  get_game_command();

  // Reset to the first game.
  selected_game = 0;

  // Show the splash
  show_game_asset(splash);

  // Sleep for the splash screen, then begin our loop
  sleep(SPLASH_DELAY_IN_SECONDS);

  do
  {
    int input = 0;
    input = read_inputs();

    // Why have "last_selected_game" when we're just +/- 
    // on selected game? So we can support a grid of games
    // at some point.
    if (input & UP) {
      if (selected_game > 0) {
        last_selected_game = selected_game;
        selected_game--;
      }
    }
    else if (input & DOWN) {
        if (selected_game < game_count) {
          last_selected_game = selected_game;
          selected_game++;
        }
    }
    
    if (input & SELECT) {
      confirm_and_start_game();
    }

    if (selected_game != last_selected_game) {
      show_game_asset(selected);
      last_selected_game = selected_game;
    }

    nanosleep(&ts, &ts);    
    /* code */
  } while (true); 

  return 0;
}
