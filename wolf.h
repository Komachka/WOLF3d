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
#define EXIT 53

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4
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

    double ray_p_x;
    double ray_p_y;
    double ray_d_x;
    double ray_d_y;
    int map_x;
    int map_y;
    double side_dist_x;
    double side_dist_y;
    double d_dist_x;
    double d_dist_y;
    double perp_wall;
    int step_x;
    int step_y;
    int side_c;
} t_rawcast;



typedef struct s_start_end_draw
{
    int start_point;
    int end_point;
}t_start_end_draw;

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

typedef struct      s_point
{
    double			x;
    double			y;
    int				colour;
}					t_point;

typedef struct      s_params
{
    void            *mlx;
    t_img           *img_struct;
    void            *window;
    double          dir_x;
    double          dir_y;
    double          rot_speed;
    double          plane_x;
    double          plane_y;
    double          move_speed;
    double          pos_x;
    double          pos_y;
    int             **map;

}t_params;

/*
** main.c
*/

void    create_base_params(t_params *params);
void    draw_flor(t_params *params);
void    draw(t_params *params);
int     get_wall_side_colour(int side_c);


/*
** rawcast_calc.c
*/

t_rawcast   calc_dist_projection_on_camera_dir(t_rawcast *link_rc);
t_rawcast   calc_par_rc(t_params *pr, t_rawcast *link_rc, int x);
t_rawcast   perform_dda(t_rawcast *l_rc, int **map);
t_rawcast   calc_step_and_side_dist(t_rawcast *link_rc);
t_start_end_draw    calc_start_and_end(double perp_wall_dist);

/*
**	key_funk.c
*/
int my_key_funk(int kkode, t_params *par);
void rorate(t_params *par, double speed, int kkode);
void move_frame(t_params *par, double dir_x, double dir_y, int kkode);
void rorate(t_params *par, double speed, int kkode);
int exit_x(void *par);

/*
**	print.c
*/

void				make_img_line(t_point *p1, t_point *p2, t_img *imege);
t_print_struct		*create_print_stuff(t_point *p1, t_point *p2);
void				print_img_line(t_print_struct *p_s,
                                   t_img *img, float t, int x);
int					put_pixel_to_image(t_point *p, t_img *image);
void                verLine(t_vline vline, t_img *img);


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
void free_map(int **map);

/*
**	map.c
*/
void create_map(t_params *params);
void mark_map(t_params *params, int i, int j);
int exit_x(void *par);
#endif

