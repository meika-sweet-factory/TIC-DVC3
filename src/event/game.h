#pragma once

#include <SDL2/SDL.h>
#include "../game.h"

void handle_game_inputs(SDL_Event *event, t_game *game);
void handle_server_events(t_game *game, char *packet);
void handle_client_events(t_conn *s, int fd, char *packet);
