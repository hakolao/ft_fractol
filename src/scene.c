/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 13:13:53 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/05 17:53:01 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int					scene_render_params(t_scene *scene,
					void *mlx, void *mlx_wdw)
{
	scene->mlx = mlx;
	scene->mlx_wdw = mlx_wdw;
	scene->pixel_bits = 32;
	scene->line_bytes = WIDTH * 4;
	scene->pixel_endian = TRUE;
	if (!(scene->frame = mlx_new_image(mlx, WIDTH, HEIGHT)) ||
		!(scene->frame_buf =
			mlx_get_data_addr(scene->frame, &scene->pixel_bits,
			&scene->line_bytes, &scene->pixel_endian)))
		return (FALSE);
	scene->redraw = TRUE;
	scene->col_r = 0;
	scene->col_g = 0;
	scene->col_b = 0;
	scene->col_a = 0;
	return (TRUE);
}

int					set_color_palette(t_scene *scene, t_rgb *palette,
					int palette_size)
{
	int		i;
	int		j;
	int		lim;
	double	mul;

	
	if (!(scene->color_palette =
		malloc(sizeof(*scene->color_palette) * scene->max_iter)))
			return (FALSE);
	i = 0;
	j = 0;
	lim = scene->max_iter / (palette_size - 1);
	while (i < scene->max_iter)
	{
		mul = (double)(i % lim) / lim;
		scene->color_palette[i] = lerp_rgb(
			COLOR(palette[j].r, palette[j].g, palette[j].b, 0),
			COLOR(palette[j + 1].r, palette[j + 1].g, palette[j + 1].b, 0),
			i != 0 && mul == 0.0 ? 1.0 : mul);
		if (i % lim == 0 && i != 0)
			j++;
		i++;
	}
	return (TRUE);
}

static t_pixel_bounds		*pixel_bounds(int x_start, int x_end, int y_start, int y_end)
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

static t_fractal_params		**thread_fractal_params(t_scene *scene)
{
	t_fractal_params		**fractal_params;
	t_thread_pixel			**pixels;
	int						size;
	int						i;
	int						j;

	size = WIDTH * HEIGHT / THREADS;
	if (!(fractal_params = malloc(sizeof(*fractal_params) * THREADS)))
		return (NULL);
	i = 0;
	while (i < THREADS)
	{
		if (!(fractal_params[i] = malloc(sizeof(**fractal_params) * size)))
			return (NULL);
		fractal_params[i]->max_iter = scene->max_iter;
		if (!(fractal_params[i]->pixel_bounds = pixel_bounds(0, WIDTH,
			i * (HEIGHT / THREADS), (i + 1) * (HEIGHT / THREADS))) ||
			!(pixels =
				malloc(sizeof(*pixels) * size)))
			return (NULL);
		j = 0;
		while (j < size)
			if (!(pixels[j++] = malloc(sizeof(**pixels))))
				return (NULL);
		fractal_params[i]->size = size;
		fractal_params[i]->pixels = pixels;
		i++;
	}
	return (fractal_params);
}

/*
** Creates a new scene containing all needed information. Set's
** camera's position based on map's size
*/

t_scene				*new_scene(void *mlx, void *mlx_wdw)
{
	t_scene		*scene;

	if ((scene = (t_scene*)malloc(sizeof(*scene))) == NULL)
		return (NULL);
	if (scene_render_params(scene, mlx, mlx_wdw) == FALSE)
		return (NULL);
	scene->mouse_right_pressed = FALSE;
	scene->mouse_left_pressed = FALSE;
	scene->shift_pressed = FALSE;
	scene->mouse_x = FALSE;
	scene->mouse_y = FALSE;
	scene->show_guide = FALSE;
	scene->max_iter = MAX_ITER;
	if (!set_color_palette(scene, (t_rgb[6]){{.r = 255, .g = 0, .b = 0},
		{.r = 255, .g = 255, .b = 0}, {.r = 0, .g = 255, .b = 0},
		{.r = 0, .g = 255, .b = 255}, {.r = 0, .g = 0, .b = 255},
		{.r = 255, .g = 0, .b = 255}}, 6))
		return (NULL);
	if (!(scene->fractal_params = thread_fractal_params(scene)))
		return (NULL);
	return (scene);
}
