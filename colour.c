/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kstorozh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/21 21:21:04 by kstorozh          #+#    #+#             */
/*   Updated: 2017/05/21 21:21:05 by kstorozh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

double	**create_gradient(int **rgb, double dx)
{
	double	**matrix_colour;

	matrix_colour = (double **)malloc(sizeof(double*) * 3);
	matrix_colour[0] = (double*)malloc(sizeof(double) * 3);
	matrix_colour[1] = (double*)malloc(sizeof(double) * 3);
	matrix_colour[2] = (double*)malloc(sizeof(double) * 3);
	matrix_colour[0][0] = (rgb[1][0] - rgb[0][0]) / dx;
	matrix_colour[0][1] = (rgb[1][1] - rgb[0][1]) / dx;
	matrix_colour[0][2] = (rgb[1][2] - rgb[0][2]) / dx;
	matrix_colour[1][0] = rgb[0][0];
	matrix_colour[1][1] = rgb[0][1];
	matrix_colour[1][2] = rgb[0][2];
	matrix_colour[2][0] = rgb[1][0];
	matrix_colour[2][1] = rgb[1][1];
	matrix_colour[2][2] = rgb[1][2];
	return (matrix_colour);
}

void	del_delta(double **matrix_colour, t_point *point)
{
	matrix_colour[2][0] -= matrix_colour[0][0];
	matrix_colour[2][1] -= matrix_colour[0][1];
	matrix_colour[2][2] -= matrix_colour[0][2];
	point->colour = (int)matrix_colour[2][0] +
			((int)matrix_colour[2][1] << 8) + ((int)matrix_colour[2][2] << 16);
}

void	add_delta(double **matrix_colour, t_point *point)
{
	matrix_colour[1][0] += matrix_colour[0][0];
	matrix_colour[1][1] += matrix_colour[0][1];
	matrix_colour[1][2] += matrix_colour[0][2];
	point->colour = (int)matrix_colour[1][0] +
			((int)matrix_colour[1][1] << 8) + ((int)matrix_colour[1][2] << 16);
}

int		*c_rgb(int colour)
{
	int	*rgb;

	if ((rgb = (int*)malloc(sizeof(int) * 3)) == NULL)
		return (NULL);
	rgb[0] = colour & 0xFF;
	rgb[1] = colour >> 8 & 0xFF;
	rgb[2] = colour >> 16 & 0xFF;
	return (rgb);
}


