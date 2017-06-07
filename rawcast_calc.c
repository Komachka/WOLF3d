#include "wolf.h"

t_rawcast   calc_dist_projection_on_camera_dir(t_rawcast *link_rc)
{
    t_rawcast   rc;

    rc = *link_rc;
    if (rc.side_c == 1 || rc.side_c == 2)
        rc.perp_wall = (rc.map_x - rc.ray_p_x +
                (1 - rc.step_x) / 2) / rc.ray_d_x;
    else
        rc.perp_wall = (rc.map_y - rc.ray_p_y +
                (1 - rc.step_y) / 2) / rc.ray_d_y;
    return rc;
}

/*
** cameraX - x-coordinate in camera space
*/

t_rawcast   calc_par_rc(t_params *pr, t_rawcast *link_rc, int x)
{
    double      cameraX;
    t_rawcast   rc;

    rc = *link_rc;
    cameraX = 2 * x / (double) SCREN_WIGHT - 1;
    rc.ray_p_x = pr->pos_x;
    rc.ray_p_y = pr->pos_y;
    rc.ray_d_x = pr->dir_x + pr->plane_x * cameraX;
    rc.ray_d_y = pr->dir_y + pr->plane_y * cameraX;
    rc.map_x = (int) rc.ray_p_x;
    rc.map_y = (int) rc.ray_p_y;
    rc.d_dist_x = sqrt(1 + (rc.ray_d_y * rc.ray_d_y) /
                                   (rc.ray_d_x * rc.ray_d_x));
    rc.d_dist_y = sqrt(1 + (rc.ray_d_x * rc.ray_d_x) /
                                   (rc.ray_d_y * rc.ray_d_y));
    return (rc);
}

t_rawcast   perform_dda(t_rawcast *l_rc, int **map)
{
    t_rawcast   rc;
    int         hit;

    rc = *l_rc;
    hit = 0;
    while (hit == 0)
    {
        if (rc.side_dist_x < rc.side_dist_y)
        {
            rc.side_dist_x += rc.d_dist_x;
            rc.map_x += rc.step_x;
            rc.side_c = (rc.step_x > 0) ? NORTH : EAST;
        }
        else
        {
            rc.side_dist_y += rc.d_dist_y;
            rc.map_y += rc.step_y;
            rc.side_c = (rc.step_y > 0) ? SOUTH :WEST;
        }
        if (map[rc.map_x][rc.map_y] > 0)
            hit = 1;
    }
    return (rc);
}

t_rawcast   calc_step_and_side_dist(t_rawcast *link_rc)
{
    t_rawcast   rc;

    rc = *link_rc;
    if (rc.ray_d_x < 0)
    {
        rc.step_x = -1;
        rc.side_dist_x = (rc.ray_p_x - rc.map_x) * rc.d_dist_x;
    }
    else
    {
        rc.step_x = 1;
        rc.side_dist_x = (rc.map_x + 1.0 - rc.ray_p_x) * rc.d_dist_x;
    }
    if (rc.ray_d_y < 0)
    {
        rc.step_y = -1;
        rc.side_dist_y = (rc.ray_p_y - rc.map_y) * rc.d_dist_y;
    }
    else
    {
        rc.step_y = 1;
        rc.side_dist_y = (rc.map_y + 1.0 - rc.ray_p_y) * rc.d_dist_y;
    }
    return (rc);
}

t_start_end_draw    calc_start_and_end(double perp_wall_dist)
{
    t_start_end_draw    result;
    int                 lineHeight;

    lineHeight = (int) ((int)SCREN_HEIGHT / perp_wall_dist);
    if (perp_wall_dist == 0)
        lineHeight = 99999999;
    result.start_point = -lineHeight / 2 + SCREN_HEIGHT / 2;
    if (result.start_point < 0)
        result.start_point = 0;
    result.end_point = lineHeight / 2 + SCREN_HEIGHT / 2;
    if (result.end_point >= SCREN_HEIGHT)
        result.end_point = SCREN_HEIGHT - 1;
    return (result);
}