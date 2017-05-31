//
// Created by Kateryna Storozh on 5/31/17.
//

#include "wolf.h"
#include <math.h>
#include <stdlib.h>
#include "wolf.h"


#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define mapWidth 24
#define mapHeight 24

#define SCREN_WIGHT 512
#define SCREN_HEIGHT 384


int worldMap[mapWidth][mapHeight]=
        {
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
                {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1},
                {1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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
                {1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

void *mlx;
void *window;
void verLine(int x, int start, int anEnd, int color);

int main(void)
{

    double posX = 22, posY = 12;  //x and y start position
    double dirX = -1, dirY = 0; //initial direction vector
    double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

    double time = 0; //time of current frame
    double oldTime = 0; //time of previous frame

    mlx = mlx_init();
    window = mlx_new_window(mlx, SCREN_WIGHT, SCREN_HEIGHT, "TEST");
        for (int x = 0; x < SCREN_WIGHT; x++) {
            //calculate ray position and direction
            double cameraX = 2 * x / (double) SCREN_WIGHT - 1; //x-coordinate in camera space
            double rayPosX = posX;
            double rayPosY = posY;
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;
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
            while (hit == 0) {
                //jump to next map square, OR in x-direction, OR in y-direction
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                //Check if ray has hit a wall
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
            if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
            else perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

            //Calculate height of line to draw on screen
            int lineHeight = (int) (SCREN_HEIGHT / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + SCREN_HEIGHT / 2;
            if (drawStart < 0)drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREN_HEIGHT / 2;
            if (drawEnd >= SCREN_HEIGHT)drawEnd = SCREN_HEIGHT - 1;

            //choose wall color

            int color;
            //printf("mapX == %d mapY = %d\n", mapX, mapY);
            printf("worldmap[x][y] = %d\n",worldMap[mapX][mapY]);
            switch (worldMap[mapX][mapY]) {
                case 1:
                    color = 0xFF0000;
                    break; //red
                case 2:
                    color = 0x00FF00;
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
            printf("colour1 = %x\n",color);
            //give x and y sides different brightness
            if (side == 1) {
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
            }

            //draw the pixels of the stripe as a vertical line

            verLine(x, drawStart, drawEnd, color);
        }

        //timing for input and FPS counter
        oldTime = time;
        time = clock();
        double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
        //print(1.0 / frameTime); //FPS counter
        //redraw();
        //cls();

        //speed modifiers
        double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
        double rotSpeed = frameTime * 3.0; //the constant value is in radians/second

        mlx_loop(mlx);

        return (0);
}

void verLine(int x, int start, int anEnd, int color)
{
    t_point *p1;
    t_point *p2;
    t_img *img;

    p1 = (t_point*)malloc(sizeof(t_point));
    p2 = (t_point*)malloc(sizeof(t_point));
    p1->colour =color;
    p2->colour =color;
    p1->x = x;
    p2->x = x;
    p1->y = start;
    p2->y = anEnd;

    make_img_line(p1, p2, img);
}