

#include <mlx.h>
#include <math.h>
#include <stdlib.h>
#include "wolf.h"


// size of tile (wall height)
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

int main()
{


  /*  // trigonometric tables
    float fSinTable[];
    float fISinTable[];
    float fCosTable[];
    float fICosTable[];
    float fTanTable[];
    float fITanTable[];
    float fFishTable[];
    float fXStepTable[];
    float fYStepTable[];*/




    int x = 400;
    int y = 400;

    void *mlx;
    void *w;

    mlx = mlx_init();
    w = mlx_new_window(mlx, x, y, "TEST");


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




    mlx_loop(mlx);
}

int get_int_col(t_rgb rgb)
{
    return ((int)rgb.r +
            ((int)rgb.g << 8) + ((int)rgb.b << 16));
}


