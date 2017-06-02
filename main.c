//
// Created by Kateryna Storozh on 5/31/17.
//

#include "wolf.h"
#include "../minilibx/mlx.h"




typedef struct s_params
{
    void *mlx;
    t_img *img;
    void *w;

    double dirX;
    double dirY;
    double rotSpeed;
    double planeX;
    double planeY;
    double moveSpeed;
    double posX;
    double posY;
    int **world_map;

}t_params;

int worldMap[mapWidth][mapHeight] =
        {
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
                {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
                {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},//
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };


void *mlx;
void *window;
//void verLine(int x, int start, int anEnd, int color);
int my_key_funk(int keykode, t_params *params);

void draw(t_params *params);

void draw_flor(t_params *params);

void verLine(t_vline vline, t_img *img);

int main(void)
{

    t_params *params;
    params = (t_params*)malloc(sizeof(t_params));
    //double posX = 22, posY = 12;  //x and y start position
    double posX = 20, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane



    mlx = mlx_init();
    window = mlx_new_window(mlx, SCREN_WIGHT, SCREN_HEIGHT, "WOLF3D");
    t_img *img_stuct;
    img_stuct = (t_img*)malloc(sizeof(t_img));
    void *img_mlx;
    img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
    create_img(img_stuct, img_mlx);


    params->posX = posX;
    params->posY = posY;
    params->planeX = planeX;
    params->planeY = planeY;
    params->dirX = dirX;
    params->dirY = dirY;

    params->img= img_stuct;
    draw_flor(params);
    draw(params);

   mlx_put_image_to_window(mlx, window, img_mlx, 0, 0);

    mlx_key_hook(window, my_key_funk, params);
    mlx_loop(mlx);
    return (0);
}

#include <stdio.h>
void draw_flor(t_params *params)
{
    int i;


    /*double cameraX = 2 * x / (double) SCREN_WIGHT - 1; //x-coordinate in camera space
    double rayDirY = params->dirY + params->planeY * cameraX;
    */
    t_point *p;

    p = (t_point*)malloc(sizeof(t_point));
    i = SCREN_HEIGHT/2;
    int y = 0;
    while (i < SCREN_HEIGHT)
    {
        y = 0;
        while(y < SCREN_WIGHT) {
            p->x = y;
            p->y = i;
            p->colour = 0x707270;

            put_pixel_to_image(p, params->img);
            //mlx_pixel_put(mlx, window, y, i, 0x707270);
            y++;
        }
        i++;
    }
}


void draw(t_params *params)
{
    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame
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


        int side_c = 0;
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
            //side_c = 1;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
            //side_c = 2;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
            //side_c = 3;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
            //side_c = 4;
        }
        //perform DDA

        side_c = 0;
        while (hit == 0) {
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
        //printf("mapX == %d mapY = %d\n", mapX, mapY);
        //printf("worldmap[x][y] = %d\n",worldMap[mapX][mapY]);
        switch (worldMap[mapX][mapY]) {
            case 1:
                color = 0xFF0000; //
                break; //red
            case 2:
                color = 0xFFFF00;
                break; //green
            case 3:
                color = 0x0000FF;
                break; //blue
            case 4:
                color = 0xFFFFFF;
                break; //white
            default:
                color = 0xc8b3c6;
                break; //yellow
        }
        //printf("colour1 = %x\n",color);
        //give x and y sides different brightness
/*        if (side == 1) {
            int b = color & 0xFF;
            int g = color >> 8 & 0xFF;
            int r = color >> 16 & 0xFF;
            //    printf("r = %d\n", r);
            //   printf("g = %d\n", g);
            //   printf("b = %d\n", b);
            r /= 2;
            g /= 2;
            b /= 2;
            //  printf("r = %d\n", r);
            //  printf("g = %d\n", g);
            // printf("b = %d\n", b);
            color = r + (g << 8) + (b << 16);
            //color = color / 2;
            //printf("colour2 = %x\n\n\n\n\n\n",color);
        }*/
        if (side == 1)
            color = 0x00FFFF;
        //draw the pixels of the stripe as a vertical line
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
        verLine(line, params->img);
        //verLine(x, drawStart, drawEnd, color);
    }

    //timing for input and FPS counter
    oldTime = time;
    time = 30;
    double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
    //print(1.0 / frameTime); //FPS counter
    //redraw();
    //cls();

    //speed modifiers
    double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 3.0; //the constant value is in radians/second
    params->moveSpeed = moveSpeed;
    params->rotSpeed = rotSpeed;
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

/*void verLine(int x, int start, int anEnd, int color)
{
    t_point *p1;
    t_point *p2;
    t_img *img_mlx;

    p1 = (t_point*)malloc(sizeof(t_point));
    p2 = (t_point*)malloc(sizeof(t_point));
    p1->colour =color;
    p2->colour =color;
    p1->x = x;
    p2->x = x;
    p1->y = start;
    p2->y = anEnd;

    make_img_line(p1, p2, img_mlx);
    free(p1);
    free(p2);
}*/

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
        mlx_destroy_image(mlx, params->img->img_mlx);
        void *img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
        params->img->img_mlx = img_mlx;
        create_img(params->img, params->img->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(mlx, window, img_mlx, 0, 0);
    }
    if (keykode == LEFT)
    {
        double oldDirX = params->dirX;
        params->dirX = params->dirX * cos(params->rotSpeed) - params->dirY * sin(params->rotSpeed);
        params->dirY = oldDirX * sin(params->rotSpeed) + params->dirY * cos(params->rotSpeed);
        double oldPlaneX = params->planeX;
        params->planeX = params->planeX * cos(params->rotSpeed) - params->planeY * sin(params->rotSpeed);
        params->planeY = oldPlaneX * sin(params->rotSpeed) + params->planeY * cos(params->rotSpeed);

        mlx_destroy_image(mlx, params->img->img_mlx);
        params->img->img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
        create_img(params->img, params->img->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(mlx, window, params->img->img_mlx, 0, 0);


        //mlx_clear_window(mlx, window);
        //draw_flor(params);
        draw(params);
    }

    if (keykode == UP)
    {
        if(worldMap[(int)(params->posX + params->dirX * params->moveSpeed)][(int)(params->posY)] == 0)
            params->posX += params->dirX * params->moveSpeed;
        if( worldMap[(int)(params->posX)][(int)(params->posY + params->dirY * params->moveSpeed)] == 0)
            params->posY += params->dirY * params->moveSpeed;


        mlx_destroy_image(mlx, params->img->img_mlx);
        params->img->img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
        create_img(params->img, params->img->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(mlx, window, params->img->img_mlx, 0, 0);


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

        mlx_destroy_image(mlx, params->img->img_mlx);
        params->img->img_mlx = mlx_new_image(mlx, SCREN_WIGHT, SCREN_HEIGHT);
        create_img(params->img, params->img->img_mlx);
        draw_flor(params);
        draw(params);
        mlx_put_image_to_window(mlx, window, params->img->img_mlx, 0, 0);

        //mlx_clear_window(mlx, window);
        //draw_flor(params);
        //draw(params);
    }
    return (0);
}