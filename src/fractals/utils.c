/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 12:29:37 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/07 15:08:42 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static t_pixel_bounds	*pixel_bounds(int x_start, int x_end, int y_start,
						int y_end)
{
	t_pixel_bounds	*bounds;

	if (!(bounds = malloc(sizeof(*bounds))))
		return (NULL);
	bounds->x_start = x_start;
	bounds->x_end = x_end;
	bounds->y_start = y_start;
	bounds->y_end = y_end;
	return (bounds);
}

static t_pixel			**thread_pixels(int size)
{
	t_pixel					**pixels;
	int						i;

	if (!(pixels = malloc(sizeof(*pixels) * size)))
		return (NULL);
	i = 0;
	while (i < size)
		if (!(pixels[i++] = malloc(sizeof(**pixels))))
			return (NULL);
	return (pixels);
}

static int				palette(t_fractal_params *params,
						t_rgb *palette, int palette_size)
{
	int		i;
	int		j;
	int		lim;
	double	mul;

	if (!(params->color_palette =
			malloc(sizeof(*params->color_palette) * COLORS)))
		return (FALSE);
	i = 0;
	j = 0;
	lim = COLORS / (palette_size - 1);
	while (i < COLORS)
	{
		mul = (double)(i % lim) / lim;
		params->color_palette[i] = lerp_rgb(
			COLOR(palette[j].r, palette[j].g, palette[j].b, 0),
			COLOR(palette[j + 1].r, palette[j + 1].g, palette[j + 1].b, 0),
			i != 0 && mul == 0.0 ? 1.0 : mul);
		if (i % lim == 0 && i != 0)
			j++;
		i++;
	}
	return (TRUE);
}

t_fractal_params		**thread_fractal_params(t_scene *scene)
{
	t_fractal_params		**fractal_params;
	int						i;

	if (!(fractal_params = malloc(sizeof(*fractal_params) * THREADS)) ||
		(HEIGHT % THREADS != 0 && log_err("HEIGHT % THREADS != 0", "Headers")))
		return (NULL);
	i = 0;
	while (i < THREADS)
	{
		if (!(fractal_params[i] = malloc(sizeof(**fractal_params) * PIXELS)))
			return (NULL);
		fractal_params[i]->max_iter = scene->max_iter;
		fractal_params[i]->zoom = 1.0;
		fractal_params[i]->size = PIXELS;
		if (!(fractal_params[i]->pixel_bounds = pixel_bounds(0, WIDTH,
			i * (HEIGHT / THREADS), (i + 1) * (HEIGHT / THREADS))) ||
			!(fractal_params[i]->pixels = thread_pixels(PIXELS)) ||
			!palette(fractal_params[i], (t_rgb[6]){{255, 0, 0}, {255, 255, 0},
			{0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {255, 0, 255}}, 6))
			return (NULL);
		i++;
	}
	return (fractal_params);
}

int						zoom(t_scene *scene, double amount, int mouse_x, int mouse_y)
{
	int	i;
	i = 0;
	(void)mouse_x;
	(void)mouse_y;
	while (i < THREADS)
	{
		scene->fractal_params[i]->zoom += amount;
		scene->fractal_params[i]->max_iter += amount > 0 ? 1 : -1;
		i++;
	}
	return (0);
}
