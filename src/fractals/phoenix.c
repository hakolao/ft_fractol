/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phoenix.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 15:07:42 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/14 16:33:38 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static double		escape_time(t_complex z, t_complex *squares,
					t_fractal_params *params)
{
	long double				iter;
	long double				xold;
	long double				yold;
	long double				xtemp;
	long double				ytemp;

	xold = 1.0;
	yold = 0.0;
	iter = 0.0;
	while (z.x * z.x + z.y * z.y <=
		params->r * params->r && iter < params->max_iter)
	{
		ytemp = 2 * z.y * z.x + params->cy + yold * params->cy;
		xtemp = z.x * z.x - z.y * z.y + params->cx + xold * params->cx;
		xold = z.x;
		yold = z.y;
		z.x = xtemp;
		z.y = ytemp;
		squares->x = z.x * z.x;
		squares->y = z.y * z.y;
		iter++;
	}
	return (iter);
}

static void			phoenix_pixel(int pixel_i, int px, int py, void *args)
{
	t_complex				z;
	t_complex				squares;
	long double				iter;
	t_fractal_params		*params;

	params = (t_fractal_params*)args;
	z = scaled_xy((t_complex){0.0, 0.0}, params, px, py);
	squares = (t_complex){z.x * z.x, z.y * z.y};
	iter = escape_time(z, &squares, params);
	set_pixel(params->pixels[pixel_i], px, py, 0);
	if (iter < params->max_iter)
		color_julia_pixel(params->pixels[pixel_i],
			iter, squares, params);
	plot_pixel_on_thread_frame(params, params->pixels[pixel_i]);
}

static void			phoenix_work(void *args)
{
	t_fractal_params *params;

	params = (t_fractal_params*)args;
	ft_pixel_foreach(params->pixel_bounds, args, phoenix_pixel);
}

void				draw_phoenix(t_scene *scene)
{
	work_parallel(THREADS, (void**)scene->fractal_params, phoenix_work);
}
