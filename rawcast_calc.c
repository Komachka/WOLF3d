#include "wolf.h"

t_rawcast calc_dist_projection_on_camera_dir(t_rawcast *link_rawcast) {
    t_rawcast rawcast;

    rawcast = *link_rawcast;
    if (rawcast.side_c == 1 || rawcast.side_c == 2)
        rawcast.perpWallDist = (rawcast.mapX - rawcast.rayPosX + (1 - rawcast.stepX) / 2) / rawcast.rayDirX;
    else
        rawcast.perpWallDist = (rawcast.mapY - rawcast.rayPosY + (1 - rawcast.stepY) / 2) / rawcast.rayDirY;
    return rawcast;
}

t_rawcast calc_base_param_rawcast(t_params *params, t_rawcast *link_rawcast, int x) {
    double cameraX;
    t_rawcast rawcast;

    rawcast = *link_rawcast;
    cameraX = 2 * x / (double) SCREN_WIGHT - 1; //x-coordinate in camera space
    rawcast.rayPosX = params->posX;
    rawcast.rayPosY = params->posY;
    rawcast.rayDirX = params->dirX + params->planeX * cameraX;
    rawcast.rayDirY = params->dirY + params->planeY * cameraX;
    rawcast.mapX = (int) rawcast.rayPosX;
    rawcast.mapY = (int) rawcast.rayPosY;
    rawcast.deltaDistX = sqrt(1 + (rawcast.rayDirY * rawcast.rayDirY) / (rawcast.rayDirX * rawcast.rayDirX));
    rawcast.deltaDistY = sqrt(1 + (rawcast.rayDirX * rawcast.rayDirX) / (rawcast.rayDirY * rawcast.rayDirY));
    return (rawcast);
}

t_rawcast perform_dda(t_rawcast *link_rawcast, int **world_map)
{
    t_rawcast rawcast;
    int hit;

    rawcast = *link_rawcast;
    hit = 0;
    while (hit == 0)
    {
        if (rawcast.sideDistX < rawcast.sideDistY)
        {
            rawcast.sideDistX += rawcast.deltaDistX;
            rawcast.mapX += rawcast.stepX;
            rawcast.side_c = (rawcast.stepX > 0) ? NORTH : EAST;
        }
        else
        {
            rawcast.sideDistY += rawcast.deltaDistY;
            rawcast.mapY += rawcast.stepY;
            rawcast.side_c = (rawcast.stepY > 0) ? SOUTH :WEST;
        }
        if (world_map[rawcast.mapX][rawcast.mapY] > 0)
            hit = 1;
    }
    return (rawcast);
}

t_rawcast calc_step_and_side_dist(t_rawcast *link_rawcast)
{
    t_rawcast rawcast;

    rawcast = *link_rawcast;
    if (rawcast.rayDirX < 0)
    {
        rawcast.stepX = -1;
        rawcast.sideDistX = (rawcast.rayPosX - rawcast.mapX) * rawcast.deltaDistX;
    }
    else
    {
        rawcast.stepX = 1;
        rawcast.sideDistX = (rawcast.mapX + 1.0 - rawcast.rayPosX) * rawcast.deltaDistX;
    }
    if (rawcast.rayDirY < 0)
    {
        rawcast.stepY = -1;
        rawcast.sideDistY = (rawcast.rayPosY - rawcast.mapY) * rawcast.deltaDistY;
    }
    else
    {
        rawcast.stepY = 1;
        rawcast.sideDistY = (rawcast.mapY + 1.0 - rawcast.rayPosY) * rawcast.deltaDistY;
    }
    return (rawcast);
}

t_start_end_draw calc_start_and_end(double perp_wall_dist) {
    t_start_end_draw result;
    int lineHeight;

    lineHeight = (int) ((int)SCREN_HEIGHT / perp_wall_dist);
    if (perp_wall_dist == 0)
        lineHeight = 99999999;
    result.drawStart = -lineHeight / 2 + SCREN_HEIGHT / 2;
    if (result.drawStart < 0)
        result.drawStart = 0;
    result.drawEnd = lineHeight / 2 + SCREN_HEIGHT / 2;
    if (result.drawEnd >= SCREN_HEIGHT)
        result.drawEnd = SCREN_HEIGHT - 1;
    return (result);
}
