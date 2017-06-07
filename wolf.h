//
// Created by Kateryna Storozh on 5/23/17.
//

#ifndef WOLF3D_WOLF_H
#define WOLF3D_WOLF_H

#include <mlx.h>
#include <stdlib.h>
#include <math.h>


#define UP 126
#define DOWN 125
#define RIGHT 124
#define LEFT 123

/*
//linux
#define UP 65362
#define DOWN 65364
#define LEFT 65361
#define RIGHT 65363
*/

#define mapWidth 24
#define mapHeight 24


#define SCREN_WIGHT 1024 //512
//#define SCREN_WIGHT 632 //512
#define SCREN_HEIGHT 1024 // 384
//#define SCREN_HEIGHT 504 // 384

typedef struct s_rawcast
{
    double cameraX;
    double rayPosX;
    double rayPosY;
    double rayDirX;
    double rayDirY;
    int mapX;
    int mapY;
    double sideDistX;
    double sideDistY;
    double deltaDistX;
    double deltaDistY;
    double perpWallDist;
    int stepX;
    int stepY;
    int hit;
    int side;
    int side_c;
    int lineHeight;
    int drawStart;
    int drawEnd;
    int color;

} t_rawcast;

typedef struct s_vline
{
    int x;
    int start;
    int end;
    int colour;
}t_vline;

typedef struct		s_print_struct
{
    int				steep1;
    int				steep2;
    double			x0;
    double			x1;
    double			y0;
    double			y1;
    int				**rgb;
}					t_print_struct;

typedef struct		s_i
{
    void			*img_mlx;
    char			*map;
    int				size_line;
}					t_img;

typedef struct
{
    double			x;
    double			y;
    int				colour;
}					t_point;

typedef struct
{
    double			r;
    double			g;
    double			b;
}					t_rgb;

typedef struct s_params
{
    void *mlx;
    t_img *img_struct;
    void *window;

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

/*
**	print.c
*/

void				make_img_line(t_point *p1, t_point *p2, t_img *imege);
t_print_struct		*create_print_stuff(t_point *p1, t_point *p2);
void				print_img_line(t_print_struct *p_s,
                                   t_img *img, float t, int x);
int					put_pixel_to_image(t_point *p, t_img *image);


/*
** colour.c
*/

int					get_colour(double h, double s, double v);
int					*c_rgb(int colour);
void				add_delta(double **matrix_colour, t_point *point);
void				del_delta(double **matrix_colour, t_point *point);
double				**create_gradient(int **rgb, double dx);


/*
**	additional.c
*/

char				*ft_toupper_str(char *str);
void				del_arrey(void **arrey, int size);
void				swap(double *x1, double *x2);
double				to_radians(double degrees);
int					what_fractal_is_it(char c);
void	            create_img(t_img *img_struct, void *img_mlx);

/*
**	map.c
*/
void create_map(t_params *params);
void mark_map(t_params *params, int i, int j);
#endif

