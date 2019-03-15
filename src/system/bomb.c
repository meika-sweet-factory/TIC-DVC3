#include "./bomb.h"

bool bomb_have_been_plant(t_map *map, t_bomb *bomb, int x, int y)
{
    if (map->matrix[y][x].bomb != NULL) {
        return false;
    }
    map->matrix[y][x].bomb = bomb;
    return true;
}

bool bomb_explosion(t_map *map, t_bomb *bomb, int x, int y)
{
    if (map->matrix[y][x].bomb == NULL) {
        return false;
    }
    for (int i = x - 2; i < x + (2 * 2 + 1); i++) {
        if (i != x) {
            if (map->matrix[y][i].bomberman != NULL) {
                destroy_bomberman(map->matrix[y][i].bomberman);
                map->matrix[y][i].bomberman = NULL;
            }
        }
    }
    for (int i = y - 2; i < y + (2 * 2 + 1); i++) {
        if (i != y) {
            if (map->matrix[i][x].bomberman != NULL) {
                destroy_bomberman(map->matrix[i][x].bomberman);
                map->matrix[i][x].bomberman = NULL;
            }
        }
    }
    destroy_bomb(map->matrix[y][x].bomb);
    map->matrix[y][x].bomb = NULL;
    return true;
}
