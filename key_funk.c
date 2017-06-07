#include "wolf.h"

void rorate(t_params *params, double speed, int keykode);

void move_frame(t_params *params, double dirX, double dirY, int keykode);

int my_key_funk(int keykode, t_params *params)
{
    void *img_mlx;

    if (keykode != RIGHT && keykode != LEFT && keykode != UP && keykode != DOWN && keykode != EXIT)
        return (0);
    if (keykode == EXIT)
        exit_x(params);
    mlx_destroy_image(params->mlx, params->img_struct->img_mlx);
    img_mlx = mlx_new_image(params->mlx, SCREN_WIGHT, SCREN_HEIGHT);
    params->img_struct->img_mlx = img_mlx;
    create_img(params->img_struct, params->img_struct->img_mlx);
    if (keykode == RIGHT || keykode == LEFT)
        rorate(params, params->rotSpeed, keykode);
    if (keykode == UP || keykode == DOWN)
        move_frame(params, params->dirX, params->dirY, keykode);
    draw_flor(params);
    draw(params);
    mlx_put_image_to_window(params->mlx, params->window, img_mlx, 0, 0);
    return (0);
}

void move_frame(t_params *params, double dirX, double dirY, int keykode)
{

    if (keykode == DOWN)
    {
        dirX = -dirX;
        dirY = -dirY;
    }
    if (params->world_map[(int) (params->posX + dirX * params->moveSpeed)][(int) (params->posY)] == 0)
            params->posX += dirX * params->moveSpeed;
    if (params->world_map[(int) (params->posX)][(int) (params->posY + dirY * params->moveSpeed)] == 0)
            params->posY += dirY * params->moveSpeed;

}
/*
** both camera direction and camera plane must be rotated
**
*/

void rorate(t_params *params, double speed, int keykode)
{
    double oldDirX;
    double oldPlaneX;

    if (keykode == RIGHT)
        speed = -speed;
    oldDirX = params->dirX;
    params->dirX = params->dirX * cos(speed) - params->dirY * sin(speed);
    params->dirY = oldDirX * sin(speed) + params->dirY * cos(speed);
    oldPlaneX = params->planeX;
    params->planeX = params->planeX * cos(speed) - params->planeY * sin(speed);
    params->planeY = oldPlaneX * sin(speed) + params->planeY * cos(speed);
}

int exit_x(void *par)
{
    t_params *params;

    params = (t_params*)par;
    free(params->world_map); // неправильно фришу нужно написать отдельную функцию
    free(params->img_struct);
    free(par);
    par = NULL;
    exit(1);
}