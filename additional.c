/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kstorozh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/21 21:12:08 by kstorozh          #+#    #+#             */
/*   Updated: 2017/05/21 21:12:35 by kstorozh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "wolf.h"



void	del_arrey(void **arrey, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (arrey[i] != NULL)
			free(arrey[i]);
		i++;
	}
	if (arrey != NULL)
		free(arrey);
}

void	swap(double *x1, double *x2)
{
	double	i;
	double	j;

	i = *x1;
	j = *x2;
	*x1 = j;
	*x2 = i;
}

double	to_radians(double degrees)
{
	return (degrees / 180.0 * M_PI);
}


void	create_img(t_img *img_struct, void *img_mlx)
{
	int		bit_per_pixel;
	int		size_line;
	int		endian;

	img_struct->map = mlx_get_data_addr(img_mlx, &bit_per_pixel,
										&size_line, &endian);
	img_struct->size_line = size_line;
	img_struct->img_mlx = img_mlx;
}
