/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kstorozh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/21 21:44:18 by kstorozh          #+#    #+#             */
/*   Updated: 2017/05/21 21:44:19 by kstorozh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			make_img_line(t_point *p1, t_point *p2, t_img *imege)
{
	t_print_struct	*ps;

	if ((ps = create_print_stuff(p1, p2)) == NULL)
		return ;
	if (fabs(ps->x0 - ps->x1) < fabs(ps->y0 - ps->y1))
	{
		swap(&ps->x0, &ps->y0);
		swap(&ps->x1, &ps->y1);
		ps->steep1 = 1;
	}
	if (ps->x0 > ps->x1)
	{
		swap(&ps->x0, &ps->x1);
		swap(&ps->y0, &ps->y1);
		ps->steep2 = 1;
	}
	print_img_line(ps, imege, 0, 0);
	del_arrey((void **)ps->rgb, 2);
}

t_print_struct	*create_print_stuff(t_point *p1, t_point *p2)
{
	t_print_struct	*print_struct;

	if ((print_struct = (t_print_struct*)malloc(sizeof(t_print_struct)))
		== NULL)
		return (NULL);
	print_struct->steep1 = 0;
	print_struct->steep2 = 0;
	print_struct->x0 = p1->x;
	print_struct->x1 = p2->x;
	print_struct->y0 = p1->y;
	print_struct->y1 = p2->y;
	if ((print_struct->rgb = (int**)malloc(sizeof(int*) * 2)) == NULL)
		return (NULL);
	print_struct->rgb[0] = c_rgb(p1->colour);
	print_struct->rgb[1] = c_rgb(p2->colour);
	return (print_struct);
}

void			print_img_line(t_print_struct *p_s, t_img *img, float t, int x)
{
	int		y;
	t_point *point;
	double	**matrix_colour;

	point = (t_point*)malloc(sizeof(t_point));
	matrix_colour = create_gradient(p_s->rgb,
									p_s->x1 - p_s->x0);
	x = (int)p_s->x0;
	while (x <= p_s->x1)
	{
		(p_s->steep2 == 1) ? del_delta(matrix_colour, point) :
		add_delta(matrix_colour, point);
		t = (float)((x - p_s->x0) /
					(float)(p_s->x1 - p_s->x0));
		y = (int)(p_s->y0 * (1.0 - t) + p_s->y1 * t);
		point->x = (p_s->steep1) ? y : x;
		point->y = (p_s->steep1) ? x : y;
		put_pixel_to_image(point, img);
		x++;
	}
	del_arrey((void **)matrix_colour, 3);
}


int				put_pixel_to_image(t_point *p, t_img *image)
{
	int		col;
	int		k;
	char	*map;
	int		size_line;


	col = p->colour;
	map = image->map;
	size_line = image->size_line;
	if (p->x >= SCREN_WIGHT || p->y >= SCREN_HEIGHT || p->x < 0 || p->y < 0) {
		return (-1);
	}
		k = (int)(ceil(p->y) * size_line + ceil(p->x) * 4);
	if (k >= SCREN_HEIGHT * size_line * 4 - 1)
		return (-1);
	else if (k < 0)
		return (-1);
	map[k] = (unsigned char)(col & 0xFF);
	k++;
	map[k] = (unsigned char)(col >> 8 & 0xFF);
	k++;
	map[k] = (unsigned char)(col >> 16 & 0xFF);
	return (0);
}

void verLine(t_vline vline, t_img *img)
{
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
