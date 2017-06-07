//
// Created by Kateryna Storozh on 5/31/17.
//

#include "wolf.h"



int my_key_funk(int keykode, t_params *params);

void draw(t_params *params);

void draw_flor(t_params *params);

void verLine(t_vline vline, t_img *img);


void create_base_params(t_params *params);

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
    mlx_key_hook(window, my_key_funk, params);
    mlx_loop(mlx);
    free(params->world_map); // неправильно фришу нужно написать отдельную функцию
    free(params->img_struct);
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
    int **worldMap;

    worldMap = params->world_map;
    double time; //time of current frame
    double oldTime; //time of previous frame
    time = 0;
    for (int x = 0; x < SCREN_WIGHT; x++) {
        //calculate ray position and direction
        double cameraX = 2 * x / (double) SCREN_WIGHT - 1; //x-coordinate in camera space
        double rayPosX = params->posX;
        double rayPosY = params->posY;
        double rayDirX = params->dirX + params->planeX * cameraX;
        double rayDirY = params->dirY + params->planeY * cameraX;

        //which box of the map we're in
        int mapX = (int) rayPosX;
        int mapY = (int) rayPosY;

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?

        //calculate step and initial sideDist

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }
        //perform DDA

        int side_c = 0;
        while (hit == 0)
        {
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                if (stepX > 0)
                    side_c = 1; ////
                if (stepX < 0)
                    side_c = 2; ////
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                if (stepY > 0)
                    side_c = 3; ////
                if (stepY < 0)
                    side_c = 4; ////
                side = 1;
            }
            //Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }

        //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
        if (side == 0)
            perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

        //Calculate height of line to draw on screen
        int lineHeight = (int) ((int)SCREN_HEIGHT / perpWallDist);
        // hz
        if (perpWallDist == 0)
            lineHeight = 99999999;
        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + SCREN_HEIGHT / 2;
        if (drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREN_HEIGHT / 2;
        if (drawEnd >= SCREN_HEIGHT)
            drawEnd = SCREN_HEIGHT - 1;

        //choose wall color


        int color;

        if (side_c == 1)
            color = 0xFF0000;
        if (side_c == 2)
            color = 0x00FF00;
        if (side_c == 3)
            color = 0x0000FF;
        if (side_c == 4)
            color = 0xFFFF00; // good
        t_vline line;
        line.x = x;
        line.start = drawStart;
        line.end = drawEnd;
        line.colour = color;
        verLine(line, params->img_struct);
        //verLine(x, drawStart, drawEnd, color);
    }

    //timing for input and FPS counter
    oldTime = time;
    time = 50;
    double frameTime = 50 / 1000.0; //frameTime is the time this frame has taken, in seconds
    //speed modifiers
    params->moveSpeed = frameTime * 5.0; // //the constant value is in squares/second
    params->rotSpeed = frameTime * 3.0; //the constant value is in radians/second
}

void verLine(t_vline vline, t_img *img) {
    t_point *p1;
    t_point *p2;

    p1 = (t_point*)malloc(sizeof(t_point));
    p2 = (t_point*)malloc(sizeof(t_point));
    p1->colour =vline.colour;
    p2->colour =vline.colour;
    p1->x = vline.x;
    p2->x = vline.x;
    p1->y = vline.start;
    p2->y = vline.end;
    make_img_line(p1, p2, img);
    free(p1);
    free(p2);
}



int my_key_funk(int keykode, t_params *params)
{

    //printf("keykode == %d\n", keykode);
    if (keykode == RIGHT)
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = params->dirX;
        params->dirX = params->dirX * cos(-params->rotSpeed) - params->dirY * sin(-params->rotSpeed);
        params->dirY = oldDirX * sin(-params->rotSpeed) + params->dirY * cos(-params->rotSpeed);
        double oldPlaneX = params->planeX;
        params->planeX = params->planeX * cos(-params->rotSpeed) - params->planeY * sin(-params->rotSpeed);
        params->planeY = oldPlaneX * sin(-params->rotSpeed) + params->planeY * cos(-params->rotSpeed);

        //mlx_clear_window(mlx, window);
        mlx_destroy_image(params->mlx, params->img_struct->img_mlx);
        void *img_mlx = mlx_new_image(params->mlx, SCREN_WIGHT, SCREN_HEIGHT);
        params->img_struct->img_mlx = img_mlx;
        create_img(params->img_struct, params->img_struct->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(params->mlx, params->window, img_mlx, 0, 0);
    }
    if (keykode == LEFT)
    {
        double oldDirX = params->dirX;
        params->dirX = params->dirX * cos(params->rotSpeed) - params->dirY * sin(params->rotSpeed);
        params->dirY = oldDirX * sin(params->rotSpeed) + params->dirY * cos(params->rotSpeed);
        double oldPlaneX = params->planeX;
        params->planeX = params->planeX * cos(params->rotSpeed) - params->planeY * sin(params->rotSpeed);
        params->planeY = oldPlaneX * sin(params->rotSpeed) + params->planeY * cos(params->rotSpeed);

        mlx_destroy_image(params->mlx, params->img_struct->img_mlx);
        params->img_struct->img_mlx = mlx_new_image(params->mlx, SCREN_WIGHT, SCREN_HEIGHT);
        create_img(params->img_struct, params->img_struct->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(params->mlx, params->window, params->img_struct->img_mlx, 0, 0);


        //mlx_clear_window(mlx, window);
        //draw_flor(params);
        draw(params);
    }

    int **worldMap = params->world_map;
    if (keykode == UP)
    {
        if(worldMap[(int)(params->posX + params->dirX * params->moveSpeed)][(int)(params->posY)] == 0)
            params->posX += params->dirX * params->moveSpeed;
        if( worldMap[(int)(params->posX)][(int)(params->posY + params->dirY * params->moveSpeed)] == 0)
            params->posY += params->dirY * params->moveSpeed;


        mlx_destroy_image(params->mlx, params->img_struct->img_mlx);
        params->img_struct->img_mlx = mlx_new_image(params->mlx, SCREN_WIGHT, SCREN_HEIGHT);
        create_img(params->img_struct, params->img_struct->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(params->mlx, params->window, params->img_struct->img_mlx, 0, 0);


        //mlx_clear_window(mlx, window);
        //draw_flor(params);
        //draw(params);
    }
    if (keykode == DOWN)
    {
        if(worldMap[(int)(params->posX - params->dirX * params->moveSpeed)][(int)(params->posY)] == 0)
            params->posX -= params->dirX * params->moveSpeed;
        if(worldMap[(int)(params->posX)][(int)(params->posY - params->dirY * params->moveSpeed)] == 0)
            params->posY -= params->dirY * params->moveSpeed;

        mlx_destroy_image(params->mlx, params->img_struct->img_mlx);
        params->img_struct->img_mlx = mlx_new_image(params->mlx, SCREN_WIGHT, SCREN_HEIGHT);
        create_img(params->img_struct, params->img_struct->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(params->mlx, params->window, params->img_struct->img_mlx, 0, 0);

        //mlx_clear_window(mlx, window);
        //draw_flor(params);
        //draw(params);
    }
    return (0);
}