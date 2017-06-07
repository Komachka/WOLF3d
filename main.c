//
// Created by Kateryna Storozh on 5/31/17.
//

#include "wolf.h"
int     main(void)
{
    t_params    *par;
    void        *img_mlx;
    void        *mlx;
    void        *win;

    par = (t_params*)malloc(sizeof(t_params));
    create_map(par);
    create_base_params(par);
    mlx = mlx_init();
    par->mlx = mlx;
    win = mlx_new_window(mlx, SCREN_WIGHT, SCREN_HEIGHT, "WOLF3D");
    par->window = win;
    img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
    create_img(par->img_struct, img_mlx);
    draw_flor(par);
    draw(par);
    mlx_put_image_to_window(mlx, win, img_mlx, 0, 0);
    mlx_hook(win, 17, 1L << 17, exit_x, par);
    mlx_hook(win, 2, 1L << 1, my_key_funk, par);
    mlx_key_hook(win, my_key_funk, par);
    mlx_loop(mlx);
    return (0);
}

/*
**	pos_x, pos_y - x and y start position
**  dir_x, dir Y - initial direction vector
**  plane_x, plane Y - the 2d raycaster version of camera plane
*/

void    create_base_params(t_params *params)
{
    t_img   *img_stuct;

    img_stuct = (t_img*)malloc(sizeof(t_img));
    params->img_struct = img_stuct;
    params->pos_x = 20;
    params->pos_y = 12;
    params->plane_x = 0;
    params->plane_y = 0.66;
    params->dir_x = -1;
    params->dir_y = 0;
}

void    draw_flor(t_params *params)
{
    int     i;
    t_point *p;
    int     y;

    p = (t_point*)malloc(sizeof(t_point));
    i = SCREN_HEIGHT/2;
    while (i < SCREN_HEIGHT)
    {
        y = 0;
        while(y < SCREN_WIGHT) {
            p->x = y;
            p->y = i;
            p->colour = 0x707270;
            put_pixel_to_image(p, params->img_struct);
            y++;
        }
        i++;
    }
    free(p);
}

void    draw(t_params *params)
{
    int                 **worldMap;
    t_rawcast           rawcast;
    t_start_end_draw    start_end_draw;
    int                 x;
    t_vline             line;

    worldMap = params->map;
    x = 0;
    while (++x < SCREN_WIGHT)
    {
        rawcast = calc_par_rc(params, &rawcast, x);
        rawcast = calc_step_and_side_dist(&rawcast);
        rawcast = perform_dda(&rawcast, worldMap);
        rawcast = calc_dist_projection_on_camera_dir(&rawcast);
        start_end_draw = calc_start_and_end(rawcast.perp_wall);
        line.x = x;
        line.start = start_end_draw.start_point;
        line.end = start_end_draw.end_point;
        line.colour = get_wall_side_colour(rawcast.side_c);
        verLine(line, params->img_struct);
    }
    params->move_speed = (30 / 1000.0) * 5.0;
    params->rot_speed = (30 / 1000.0) * 3.0;
}

int     get_wall_side_colour(int side_c)
{
    int color;

    if (side_c == NORTH)
        color = 0xFF0000;
    if (side_c == EAST)
        color = 0x00FF00;
    if (side_c == SOUTH)
        color = 0x0000FF;
    if (side_c == WEST)
        color = 0xFFFF00;
    return (color);
}