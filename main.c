
#include <math.h>
#include <stdlib.h>
#include "wolf.h"


#include <stdio.h>
#include <unistd.h>

void drawBackground();
void fillRect(int x, int y, int wight, int height, int color);
void render();
int get_int_col(t_rgb rgb);
// size of tile (wall height)

typedef struct s_points
{
    double x;
    double y;
    int col;
} t_poinss;
void fill(t_poinss p, int height, int wight);

void drawOverheadMap();

#define TILE_SIZE  64
#define WALL_HEIGHT  64
#define PROJECTIONPLANEWIDTH  320
#define PROJECTIONPLANEHEIGHT  200
#define ANGLE60  PROJECTIONPLANEWIDTH
#define ANGLE30 (ANGLE60/2)
#define ANGLE15  (ANGLE30/2)
#define ANGLE90  (ANGLE30*3)
#define ANGLE180  (ANGLE90*2)
#define ANGLE270  (ANGLE90*3)
#define ANGLE360  (ANGLE60*6)
#define ANGLE0  0
#define ANGLE5  (ANGLE30/6)
#define ANGLE10  (ANGLE5*2)

void *mlx;
void *win;
// player's attributes
int fPlayerX = 100;
int fPlayerY = 160;
int fPlayerArc = ANGLE0;
int fPlayerDistanceToTheProjectionPlane = 277;
int fPlayerHeight = 32;
int fPlayerSpeed = 16;
int fProjectionPlaneYCenter = PROJECTIONPLANEHEIGHT/2;
// the following variables are used to keep the player coordinate in the overhead map
int fPlayerMapX, fPlayerMapY, fMinimapWidth;


// 2 dimensional map
int *fMap;
int w = 1;
int o = 0;

/*
byte fMap[];
byte W=1;                                // wall
byte O=0;                                // opening
*/
int MAP_WIDTH=12;
int MAP_HEIGHT=12;

int map [12*12] =
        {
                1,1,1,1,1,1,1,1,1,1,1,1,
                1,0,0,0,0,0,0,0,0,0,0,1,
                1,0,0,0,0,0,0,0,0,0,0,1,
                1,0,0,0,0,0,0,0,1,0,0,1,
                1,0,0,1,0,1,0,0,1,0,0,1,
                1,0,0,1,0,1,1,0,1,0,0,1,
                1,0,0,1,0,0,1,0,1,0,0,1,
                1,0,0,0,1,0,1,0,1,0,0,1,
                1,0,0,0,1,0,1,0,1,0,0,1,
                1,0,0,0,1,1,1,0,1,0,0,1,
                1,0,0,0,0,0,0,0,0,0,0,1,
                1,1,1,1,1,1,1,1,1,1,1,1
        };

int main()
{


    // trigonometric tables
    int i = 0;
    float radian;
    float fSinTable[ANGLE360 + 1];
    float fISinTable[ANGLE360 + 1];
    float fCosTable[ANGLE360 + 1];
    float fICosTable[ANGLE360 + 1];
    float fTanTable[ANGLE360 + 1];
    float fITanTable[ANGLE360 + 1];
    float fFishTable[ANGLE360 + 1];
    float fXStepTable[ANGLE360 + 1];
    float fYStepTable[ANGLE360 + 1];

    for (i=0; i<=ANGLE360;i++)
    {
      // get the radian value (the last addition is to avoid division by 0, try removing
          // that and you'll see a hole in the wall when a ray is at 0, 90, 180, or 270 degree)
      radian = to_radians((double)i) + (float)(0.0001);
      fSinTable[i]=(float)sin(radian);
      fISinTable[i]=(1.0F/(fSinTable[i]));
      fCosTable[i]=(float)cos(radian);
      fICosTable[i]=(1.0F/(fCosTable[i]));
      fTanTable[i]=(float)tan(radian);
      fITanTable[i]=(1.0F/fTanTable[i]);


    // facing left
      if (i>=ANGLE90 && i<ANGLE270)
      {
        fXStepTable[i] = (float)(TILE_SIZE/fTanTable[i]);
        if (fXStepTable[i]>0)
          fXStepTable[i]=-fXStepTable[i];
      }
      // facing right
      else
      {
        fXStepTable[i] = (float)(TILE_SIZE/fTanTable[i]);
        if (fXStepTable[i]<0)
          fXStepTable[i]=-fXStepTable[i];
      }

      // FACING DOWN
      if (i>=ANGLE0 && i<ANGLE180)
      {
        fYStepTable[i] = (float)(TILE_SIZE*fTanTable[i]);
        if (fYStepTable[i]<0)
          fYStepTable[i]=-fYStepTable[i];
      }
      // FACING UP
      else
      {
        fYStepTable[i] = (float)(TILE_SIZE*fTanTable[i]);
        if (fYStepTable[i]>0)
          fYStepTable[i]=-fYStepTable[i];
      }
    }

    for (i=-ANGLE30; i<=ANGLE30; i++)
    {
        radian = to_radians(i);
        // we don't have negative angle, so make it start at 0
        // this will give range 0 to 320
        fFishTable[i+ANGLE30] = (float)(1.0F/cos(radian));
    }




    int x = PROJECTIONPLANEWIDTH;
    int y = PROJECTIONPLANEHEIGHT;


    mlx = mlx_init();
    win = mlx_new_window(mlx, x, y, "TEST");




     // movement flag
  int fKeyUp=0;
  int fKeyDown=0;
  int fKeyLeft=0;
  int fKeyRight=0;



fMap = map;


float playerXDir=fCosTable[fPlayerArc];
float playerYDir=fSinTable[fPlayerArc];

render();
    printf("HETETETE!!!\n");
mlx_loop(mlx);
}

void drawBackground()
{
    // sky
        int c=25;
        int r;
        t_rgb rgb;
        rgb = (t_rgb){.r = 255, .g = 0, .b =  255};
        for (r=0; r<PROJECTIONPLANEHEIGHT/2; r+=10)
        {
            printf("1r = %d\n", r);

            fillRect(0, r, PROJECTIONPLANEWIDTH, 10, get_int_col(rgb));
            rgb.g += 10;
            //fillRect(0, r, PROJECTIONPLANEWIDTH, 10, color);
                c+=20;
        }
        // ground
        c=22;
        int color = 0xff0000;
        for (; r<PROJECTIONPLANEHEIGHT; r+=15)
        {
            printf("2r = %d\n", r);
            fillRect(0, r, PROJECTIONPLANEWIDTH, 15, color);
                //fOffscreenGraphics.setColor(new Color(c, 20 20));
                //fOffscreenGraphics.fillRect(0, r, PROJECTIONPLANEWIDTH, 15);
                c += 15;
        }
}



void fillRect(int x, int y, int wight, int height, int color)
{
    t_poinss p;
    /*if (x > wight)
    {
        p.x = wight;
        wight = x;
    }*/
  /*  if (y > height) {
        p.y = height;
        height = y;
    }*/

    p.x = x;
    p.y = y;
    p.col = color;
    fill(p, height + y, wight + x);

}

void fill(t_poinss p, int height, int wight) {
    int x = 0;
    int y = 0;
    int dx = 1;
    int dy = 1;

    x = (int)p.x;
    y = (int)p.y;
    int tmp_x = x;
    //height += p.y;
    //wight += p.x;
    //printf("x0 = %f y0 = %f, h = %d w = %d\n", p.x, p.y, height, wight);

    while (y < height)
    {
        x = tmp_x;

        while (x < wight)
        {
            mlx_pixel_put(mlx, win, x, y, p.col);
            printf("x = %d y = %d\n", x, y);
            x+=dx;
        }
        //printf("%d\n", y);

        y += dy;
    }

    /*static int i = 0;
    //printf("i = %d\n", i++);
    printf("x = %d y = %d\n", (int)p.x, (int)p.y);

    if (p.x >=wight )
        return;
    if (p.y >= height)
        return;
    if (p.x <= 0 || p.y <= 0)
        return;
    mlx_pixel_put(mlx, win, (int)p.x, (int)p.y, p.col);
    double tmp = p.x;
    p.x +=1;
    fill(p, height, wight);
    p.x = tmp;
    p.y +=1;
    fill(p, height, wight);*/
}

void render()
{
    drawBackground();
    drawOverheadMap();

}

void drawOverheadMap() {
    fMinimapWidth=5;
    int col;
    for (int u=0; u<MAP_WIDTH; u++)
    {
        for (int v=0; v<MAP_HEIGHT; v++)
        {
            if (fMap[v*MAP_WIDTH+u]==1)
            {
                col = 0xFF00FF;
                //fOffscreenGraphics.setColor(Color.cyan);
            }
            else
            {
                col = 0x00FF00;
                //fOffscreenGraphics.setColor(Color.black);
            }
            printf("\n\n\n\n\nMAP\n");
            double x0 = PROJECTIONPLANEWIDTH+(u*fMinimapWidth);
            double y0 = v*fMinimapWidth;
            double wid = fMinimapWidth;
            double hei = fMinimapWidth;
            printf("x0 = %f y0  = %f x = %f y = %f\n", x0, y0, wid, hei);
            fillRect(x0,y0 , wid, hei, col);
        }
    }
    fPlayerMapX=PROJECTIONPLANEWIDTH+(int)(((float)fPlayerX/(float)TILE_SIZE) * fMinimapWidth);
    fPlayerMapY=(int)(((float)fPlayerY/(float)TILE_SIZE) * fMinimapWidth);
}




int get_int_col(t_rgb rgb)
{
    return ((int)rgb.r +
            ((int)rgb.g << 8) + ((int)rgb.b << 16));
}


