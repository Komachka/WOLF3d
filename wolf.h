//
// Created by Kateryna Storozh on 5/23/17.
//

#ifndef WOLF3D_WOLF_H
#define WOLF3D_WOLF_H

#include "../minilibx/mlx.h"
#include <stdlib.h>
#include <math.h>

#define mapWidth 24
#define mapHeight 24

extern void* mlx;
extern void* win;


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
    void			*img;
    char			*map;
    int				size_line;
    double			scale;
    double			zoom;
    double			move_x;
    double			move_y;
    double			iterations;
    double			a;
    double			size;
    double			c_rial;
    double			c_imegian;
    double			alpha;
    double			beta;
    int				season;
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

#endif

