/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 14:48:25 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/14 15:02:58 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void			draw_power_info(t_scene *scene, int x, int y)
{
	char	*n_pow;

	if ((scene->artist == mandelbrot_n || scene->artist == julia_n) &&
		(n_pow = ft_itoa(scene->fractal_params[0]->pow_n)))
	{
		mlx_string_put(scene->mlx, scene->mlx_wdw, x, y + 40,
			UI_COLOR, "Pow n: ");
		mlx_string_put(scene->mlx, scene->mlx_wdw, x + 100, y + 40, UI_COLOR,
		n_pow);
		ft_strdel(&n_pow);
	}
}

void			draw_cx_cy_sign_info(t_scene *scene, int x, int y)
{
	char	*cx_sign;
	char	*cy_sign;

	if ((scene->artist == julia_mod) &&
		(cx_sign = ft_itoa(scene->fractal_params[0]->cx_sign)) &&
		(cy_sign = ft_itoa(scene->fractal_params[0]->cy_sign)))
	{
		mlx_string_put(scene->mlx, scene->mlx_wdw, x, y + 40,
			UI_COLOR, "Cx sign: ");
		mlx_string_put(scene->mlx, scene->mlx_wdw, x + 100, y + 40, UI_COLOR,
		cx_sign);
		mlx_string_put(scene->mlx, scene->mlx_wdw, x, y + 60,
			UI_COLOR, "Cy sign: ");
		mlx_string_put(scene->mlx, scene->mlx_wdw, x + 100, y + 60, UI_COLOR,
		cy_sign);
		ft_strdel(&cx_sign);
		ft_strdel(&cy_sign);
	}
}