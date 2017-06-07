#include "wolf.h"

void create_map(t_params *params)
{
    int i;
    int j;

    i = 0;
    params->world_map = (int **)malloc(sizeof(int *) * mapHeight);
    while (i < (int)mapHeight)
    {
        params->world_map[i] = (int*)malloc(sizeof(int) * mapWidth);
        j = 0;
        while(j < (int)mapWidth)
        {
            mark_map(params, i, j);
            j++;
        }
        i++;
    }
}

void mark_map(t_params *params, int i, int j)
{
    if (i == 0 || j == 0 || i == mapWidth - 1 || j == mapHeight - 1)
        params->world_map[i][j] = 1;
    else
        params->world_map[i][j] = 0;
    if (i * j == 5 || i * j ==  8 || i * j ==  13 || i * j == 21 || i * j == 34|| i * j == 55||
        i * j == 89 || i * j ==  144 || i * j == 233 || i * j ==  377)
        params->world_map[i][j] = 1;
    if ((i == 5 && j != 1) || (i == 5 && j != 2) || (i == 5 && j != 3) || ( i == 5 && j != 4) || ( i == 5 && j != 5))
        params->world_map[i][j] = 1;
    if (i%2 == 0 && j < mapWidth/2)
        params->world_map[i][j] = 1;
}

