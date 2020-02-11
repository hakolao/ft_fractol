// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   julia.c                                            :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2020/02/11 16:03:20 by ohakola           #+#    #+#             */
// /*   Updated: 2020/02/11 16:08:32 by ohakola          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "fractol.h"

// static void		color_fractal_pixel(t_pixel *pixel, double iter,
// 				double r_i_z_square[3], t_fractal_params *params)
// {
// 	double				log_zn;

// 	log_zn = log(r_i_z_square[0] + r_i_z_square[1]) / 2.0;
// 	iter = iter + 1.0 - log(log_zn / log(2.0)) / log(2.0);
// 	pixel->color = lerp_rgb(params->color_palette[
// 			(int)floor(iter) % params->palette_size],
// 		params->color_palette[
// 			(int)((floor(iter)) + 1) % params->palette_size],
// 		iter - floor(iter));
// }

// /*
// ** Centers of interest:
// ** http://paulbourke.net/fractals/mandelbrot/
// */

// static double	*scaled_xy(double *x0_y0, t_fractal_params *params,
// 				int px, int py)
// {
// 	double	min_x0;
// 	double	min_y0;
// 	double	max_x0;
// 	double	max_y0;

// 	min_x0 = params->min_x;
// 	max_x0 = params->max_y;
// 	min_y0 = params->min_y;
// 	max_y0 = params->max_y;
// 	x0_y0[0] = (min_x0 + px * (max_x0 - min_x0) / WIDTH) *
// 		params->zoom_mul + params->center_x;
// 	x0_y0[1] = (min_y0 + py * (max_y0 - min_y0) / HEIGHT) *
// 		params->zoom_mul + params->center_y;
// 	return (x0_y0);
// }

// static void		julia_pixel(int pixel_i, int px, int py, void *args)
// {
// 	double				xy[2];
// 	double				r;
// 	double				*x0_y0;
// 	double				iter;
// 	t_fractal_params	*params;

// 	params = (t_fractal_params*)args;
// 	x0_y0 = scaled_xy((double[2]){0.0}, params, px, py);
// 	iter = 0.0;
// 	while (x0_y0[0] * x0_y0[0] + x0_y0[1] * x0_y0[1] < r * r &&
// 		iter < (params->max_iter))
// 	{

// 		iter++;
// 	}
// 	set_pixel(params->pixels[pixel_i], px, py, 0);
// 	if (iter < params->max_iter)
// 		color_fractal_pixel(params->pixels[pixel_i], iter,
// 			r_i_z_square, params);
// 	plot_pixel_on_thread_frame(params, params->pixels[pixel_i]);
// }

// static void		julia_work(void *args)
// {
// 	t_fractal_params *params;

// 	params = (t_fractal_params*)args;
// 	ft_pixel_foreach(params->pixel_bounds, args, julia_pixel);
// }

// void			draw_julia(t_scene *scene)
// {
// 	work_parallel(THREADS, (void**)scene->fractal_params, julia_work);
// }
