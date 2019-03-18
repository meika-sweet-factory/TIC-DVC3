#pragma once

#include <SDL2/SDL.h>
#include "./ressource.h"
#include "./ui/menu.h"
#include "./game/map.h"

void render_entites(SDL_Renderer *renderer, t_ressource *resssource, t_map *map);
void render_menu(SDL_Renderer *renderer, t_ressource *resssource, t_menu *menu);
