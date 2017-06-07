//
// Created by Kateryna Storozh on 5/31/17.
//

#include "wolf.h"



int my_key_funk(int keykode, t_params *params);

void draw(t_params *params);

void draw_flor(t_params *params);

void verLine(t_vline vline, t_img *img);


void create_base_params(t_params *params);

int get_wall_side_colour(int c);

t_start_end_draw calc_start_and_end(double perp_wall_dist);

t_rawcast calc_step_and_side_dist(t_rawcast *link_rawcast);

t_rawcast perform_dda(t_rawcast *link_rawcast, int **world_map);

t_rawcast calc_base_param_rawcast(t_params *params, t_rawcast *link_rawcast, int x);

t_rawcast calc_dist_projection_on_camera_dir(t_rawcast *link_rawcast);

int main(void)
{

    t_params *params;
    void *img_mlx;
    void *mlx;
    void *window;

    params = (t_params*)malloc(sizeof(t_params));
    create_map(params);
    create_base_params(params);
    mlx = mlx_init();
    params->mlx = mlx;
    window = mlx_new_window(mlx, SCREN_WIGHT, SCREN_HEIGHT, "WOLF3D");
    params->window = window;
    img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
    create_img(params->img_struct, img_mlx);
    draw_flor(params);
    draw(params);
    mlx_put_image_to_window(mlx, window, img_mlx, 0, 0);
    mlx_hook(window, 17, 1L << 17, exit_x, params);
    mlx_hook(window, 2, 1L << 1, my_key_funk, params);
    mlx_key_hook(window, my_key_funk, params);
    mlx_loop(mlx);
    free(params);

    return (0);
}

/*
**	posX, posY - x and y start position
**  dirX, dir Y - initial direction vector
**  planeX, plane Y - the 2d raycaster version of camera plane
*/

void create_base_params(t_params *params)
{
    t_img *img_stuct;

    img_stuct = (t_img*)malloc(sizeof(t_img));
    params->img_struct= img_stuct;
    params->posX = 20;
    params->posY = 12;
    params->planeX = 0;
    params->planeY = 0.66;
    params->dirX = -1;
    params->dirY = 0;
}


void draw_flor(t_params *params)
{
    int     i;
    t_point *p;
    int     y;

    p = (t_point*)malloc(sizeof(t_point));
    i = SCREN_HEIGHT/2;
    y = 0;
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
}


void draw(t_params *params)
{
    int                 **worldMap;
    t_rawcast           rawcast;
    t_start_end_draw    start_end_draw;
    double              frameTime;

    worldMap = params->world_map;
    for (int x = 0; x < SCREN_WIGHT; x++) {
        rawcast = calc_base_param_rawcast(params, &rawcast, x);
        rawcast = calc_step_and_side_dist(&rawcast);
        rawcast = perform_dda(&rawcast, worldMap);
        rawcast = calc_dist_projection_on_camera_dir(&rawcast);
        start_end_draw = calc_start_and_end(rawcast.perpWallDist);
        t_vline line; //
        line.x = x;
        line.start = start_end_draw.drawStart;
        line.end = start_end_draw.drawEnd;
        line.colour = get_wall_side_colour(rawcast.side_c);
        verLine(line, params->img_struct);
    }
    frameTime = 30 / 1000.0;
    params->moveSpeed = frameTime * 5.0;
    params->rotSpeed = frameTime * 3.0;
}



int get_wall_side_colour(int side_c) {
    int color;

    if (side_c == 1)
        color = 0xFF0000;
    if (side_c == 2)
        color = 0x00FF00;
    if (side_c == 3)
        color = 0x0000FF;
    if (side_c == 4)
        color = 0xFFFF00; // good
    return (color);
}





