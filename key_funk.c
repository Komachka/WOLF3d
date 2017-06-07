#include "wolf.h"

int my_key_funk(int kkode, t_params *par)
{
    void *img_mlx;

    if (kkode != RIGHT && kkode != LEFT && kkode != UP
        && kkode != DOWN && kkode != EXIT)
        return (0);
    if (kkode == EXIT)
        exit_x(par);
    mlx_destroy_image(par->mlx, par->img_struct->img_mlx);
    img_mlx = mlx_new_image(par->mlx, SCREN_WIGHT, SCREN_HEIGHT);
    par->img_struct->img_mlx = img_mlx;
    create_img(par->img_struct, par->img_struct->img_mlx);
    if (kkode == RIGHT || kkode == LEFT)
        rorate(par, par->rot_speed, kkode);
    if (kkode == UP || kkode == DOWN)
        move_frame(par, par->dir_x, par->dir_y, kkode);
    draw_flor(par);
    draw(par);
    mlx_put_image_to_window(par->mlx, par->window, img_mlx, 0, 0);
    return (0);
}

void move_frame(t_params *par, double dir_x, double dir_y, int kkode)
{

    if (kkode == DOWN)
    {
        dir_x = -dir_x;
        dir_y = -dir_y;
    }
    if (par->map[(int) (par->pos_x + dir_x * par->move_speed)][(int) (par->pos_y)] == 0)
            par->pos_x += dir_x * par->move_speed;
    if (par->map[(int) (par->pos_x)][(int) (par->pos_y + dir_y * par->move_speed)] == 0)
            par->pos_y += dir_y * par->move_speed;

}
/*
** both camera direction and camera plane must be rotated
*/

void rorate(t_params *par, double speed, int kkode)
{
    double old_x;
    double old_planex;

    if (kkode == RIGHT)
        speed = -speed;
    old_x = par->dir_x;
    par->dir_x = par->dir_x * cos(speed) - par->dir_y * sin(speed);
    par->dir_y = old_x * sin(speed) + par->dir_y * cos(speed);
    old_planex = par->plane_x;
    par->plane_x = par->plane_x * cos(speed) - par->plane_y * sin(speed);
    par->plane_y = old_planex * sin(speed) + par->plane_y * cos(speed);
}

int exit_x(void *par)
{
    t_params *params;

    params = (t_params*)par;
    free_map(params->map);
    free(params->img_struct);
    free(params);
    par = NULL;
    exit(1);
}

