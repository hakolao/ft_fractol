/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 10:46:22 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/07 15:00:35 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void		color_fractal_pixel(t_pixel *pixel, double iter,
				double r_i_z_square[3], int *color_palette)
{
	double				log_zn;

	log_zn = log(r_i_z_square[0] + r_i_z_square[1]) / 2.0;
	iter = iter + 1.0 - log(log_zn / log(2.0)) / log(2.0);
	pixel->color = lerp_rgb(color_palette[(int)floor(iter) % COLORS],
		color_palette[(int)((floor(iter)) + 1) % COLORS],
		iter - floor(iter));
}

static double	*scaled_xy(double *x0_y0, t_fractal_params *params,
				int px, int py)
{
	double	min_x;
	double	min_y;
	double	max_x;
	double	max_y;

	min_x = -2.5;
	max_x = 1.0;
	min_y = -1.0;
	max_y = 1.0;
	x0_y0[0] = ft_lmap_double((double)px, (double[2]){0, (double)WIDTH},
		(double[2]){min_x,max_x});
	x0_y0[1] = ft_lmap_double((double)py, (double[2]){0, (double)HEIGHT},
		(double[2]){min_y,max_y});
	(void)params;
	return (x0_y0);
}

static void		mandelbrot_pixel(int pixel_i, int px, int py, void *args)
{
	double				xy[2];
	double				*x0_y0;
	double				*r_i_z_square;
	double				iter;
	t_fractal_params	*params;

	params = (t_fractal_params*)args;
	r_i_z_square = (double[3]){0.0};
	x0_y0 = scaled_xy((double[2]){0.0}, params, px, py);
	iter = 0.0;
	while (r_i_z_square[0] + r_i_z_square[1] <= (1 << 16) &&
		iter < (params->max_iter))
	{
		xy[0] = r_i_z_square[0] - r_i_z_square[1] + x0_y0[0];
		xy[1] = r_i_z_square[2] - r_i_z_square[0] - r_i_z_square[1] + x0_y0[1];
		r_i_z_square[0] = xy[0] * xy[0];
		r_i_z_square[1] = xy[1] * xy[1];
		r_i_z_square[2] = (xy[0] + xy[1]) * (xy[0] + xy[1]);
		iter++;
	}
	set_pixel(params->pixels[pixel_i], px, py, COLOR(0, 0, 0, 0));
	if (iter < params->max_iter)
		color_fractal_pixel(params->pixels[pixel_i], iter, r_i_z_square,
			params->color_palette);
}

static void		mandelbrot_work(void *args)
{
	t_fractal_params *params;

	params = (t_fractal_params*)args;
	ft_pixel_foreach(params->pixel_bounds, args, mandelbrot_pixel);
}

void			draw_mandelbrot(t_scene *scene)
{
	work_parallel(THREADS, (void**)scene->fractal_params, mandelbrot_work);
	plot_threaded_pixels(scene);
}
