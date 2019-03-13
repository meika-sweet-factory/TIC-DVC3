#include <string.h>
#include "./game.h"
#include "../net.h"
#include "../net/conn.h"
#include "../system/bomb.h"
#include "../system/bomberman.h"
#include "../compute/movement.h"

void        handle_game_inputs(SDL_Event *event, t_game *game)
{
    char    packet[FIXED_PACKET_LENGHT];
    int     x = 0;
    int     y = 0;

    switch (event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_UP:
                    y = compute_bomberman_up_move(game->map, game->player->y);
                    if (move_bomberman(game->map, game->player->x, game->player->y, game->player->x, y)) {
                        sprintf(packet, "mv 1 %d %d", game->player->x, game->player->y);
                        send_event(game->server, packet);
                        game->player->y = y;
                    }
                    break;

                case SDLK_DOWN:
                    y = compute_bomberman_down_move(game->map, game->player->y);
                    if (move_bomberman(game->map, game->player->x, game->player->y, game->player->x, y)) {
                        sprintf(packet, "mv 2 %d %d", game->player->x, game->player->y);
                        send_event(game->server, packet);
                        game->player->y = y;
                    }
                    break;

                case SDLK_LEFT:
                    x = compute_bomberman_left_move(game->map, game->player->x);
                    if (move_bomberman(game->map, game->player->x, game->player->y, x, game->player->y)) {
                        sprintf(packet, "mv 3 %d %d", game->player->x, game->player->y);
                        send_event(game->server, packet);
                        game->player->x = x;
                    }
                    break;

                case SDLK_RIGHT:
                    x = compute_bomberman_right_move(game->map, game->player->x);
                    if (move_bomberman(game->map, game->player->x, game->player->y, x, game->player->y)) {
                        sprintf(packet, "mv 4 %d %d", game->player->x, game->player->y);
                        send_event(game->server, packet);
                        game->player->x = x;
                    }
                    break;

                case SDLK_SPACE:
                    if (allahu_akbar(game->map, create_bomb(BOMB_BASIC_TYPE, SDL_GetTicks()), game->player->x, game->player->y)) {
                        sprintf(packet, "spawn 2 1 %02d %02d", game->player->x, game->player->y);
                        send_event(game->server, packet);
                    }
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void handle_game_internal_events(t_game *game)
{
    for (size_t i = 0; i < game->map->height; i++) {
        for (size_t j = 0; j < game->map->width; j++) {
            if (game->map->matrix[i][j].bomb != NULL) {
                if (SDL_GetTicks() - game->map->matrix[i][j].bomb->drop_time > 5000) {
                    bomb_explosion(game->map, game->map->matrix[i][j].bomb, j, i);
                }
            }
        }
    }
}

void        handle_game_server_events(t_game *game, char *packet)
{
    int     x = 0;
    int     y = 0;

    if (strncmp(packet, "spawn", 5) == 0) {
        if (sscanf(packet, "spawn 1 %02d %02d", &x, &y) == 2) {
            if (place_bomberman(game->map, create_bomberman(), x, y)) {
                char discorvery_packet[FIXED_PACKET_LENGHT];

                printf("A new player have been connected at %d:%d\n", x, y);
                sprintf(discorvery_packet, "spawn 1 %02d %02d", game->player->x, game->player->y);
                send_event(game->server, discorvery_packet);
            }
        } else if (sscanf(packet, "spawn 2 1 %02d %02d", &x, &y) == 2) {
            allahu_akbar(game->map, create_bomb(BOMB_BASIC_TYPE, SDL_GetTicks()), x, y);
        }
    } else if (strncmp(packet, "mv", 2) == 0) {
        if (sscanf(packet, "mv 1 %02d %02d", &x, &y) == 2) {
            move_bomberman(game->map, x, y, x, compute_bomberman_up_move(game->map, y));
        } else if (sscanf(packet, "mv 2 %02d %02d", &x, &y) == 2) {
            move_bomberman(game->map, x, y, x, compute_bomberman_down_move(game->map, y));
        } else if (sscanf(packet, "mv 3 %02d %02d", &x, &y) == 2) {
            move_bomberman(game->map, x, y, compute_bomberman_left_move(game->map, x), y);
        } else if (sscanf(packet, "mv 4 %02d %02d", &x, &y) == 2) {
            move_bomberman(game->map, x, y, compute_bomberman_right_move(game->map, x), y);
        }
    }
}
