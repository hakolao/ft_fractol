/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 15:03:35 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/25 14:14:19 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void			draw_whole_palette(t_scene *scene, int x, int y)
{
	int		i;
	int		px;
	int		py;
	int		color;
	int		x_len;

	i = -1;
	x_len = ceil((double)WIDTH / scene->palette_size);
	while (++i < scene->palette_size)
	{
		color = scene->fractal_params[0]->palette[i];
		py = y - 1;
		while (++py < y + 30)
		{
			px = x + i * x_len - 1;
			while (++px < x + i * x_len + x_len)
				mlx_pixel_put(scene->mlx, scene->mlx_wdw, px, py, color);
		}
	}
}

static void			draw_palette(t_scene *scene, int i, int x, int y)
{
	int		color;
	char	*nbstr;
	int		px;
	int		py;

	mlx_string_put(scene->mlx, scene->mlx_wdw, x, y,
		scene->ui_color, "Palette, total colors: ");
	if (!(nbstr = ft_itoa(scene->palette_size)))
		return ;
	mlx_string_put(scene->mlx, scene->mlx_wdw, x + 250, y,
		scene->ui_color, nbstr);
	while (i < scene->colors_size)
	{
		color = COLOR(scene->colors[i]->r,
			scene->colors[i]->g, scene->colors[i]->b, 0);
		py = y + 30 - 1;
		while (++py < y + 30 + 50)
		{
			px = x + i * 60 - 1;
			while (++px < x + i * 60 + 50)
				mlx_pixel_put(scene->mlx, scene->mlx_wdw, px, py, color);
		}
		i++;
	}
	ft_strdel(&nbstr);
}

static void			draw_center_info(t_scene *scene, int x, int y)
{
	char	*x_pos;
	char	*y_pos;

	if (!(x_pos = ft_ftoa(scene->fractal_params[0]->center_x, 15)) ||
		!(y_pos = ft_ftoa(scene->fractal_params[0]->center_y, 15)))
		return ;
	mlx_string_put(scene->mlx, scene->mlx_wdw, x, y,
		scene->ui_color, "Center x: ");
	mlx_string_put(scene->mlx, scene->mlx_wdw, x + 100, y,
		scene->ui_color, x_pos);
	mlx_string_put(scene->mlx, scene->mlx_wdw, x, y + 20, scene->ui_color,
		"Center y: ");
	mlx_string_put(scene->mlx, scene->mlx_wdw, x + 100, y + 20, scene->ui_color,
		y_pos);
	ft_strdel(&x_pos);
	ft_strdel(&y_pos);
}

static void			draw_info(t_scene *scene, int x, int y)
{
	draw_center_info(scene, x, y);
	draw_power_info(scene, x, y);
	draw_cx_cy_info(scene, x, y);
	draw_px_py_info(scene, x, y);
}

void				draw_ui(t_scene *scene)
{
	char	*guidestr;
	char	*iterations;
	char	*zoom;
	char	*iterstr;
	char	*zoomstr;

	if (!scene->show_guide)
		return ;
	if (!(guidestr = guide()) ||
		!(iterations = ft_itoa(scene->fractal_params[0]->max_iter)) ||
		!(iterstr = ft_strjoin("Max iters: ", iterations)) ||
		!(zoom = ft_ftoa(scene->fractal_params[0]->zoom, 15)) ||
		!(zoomstr = ft_strjoin("Zoom: ", zoom)))
		return ;
	draw_paragraph(scene, guidestr, 10, 30);
	draw_paragraph(scene, iterstr, WIDTH - 300, 30);
	draw_paragraph(scene, zoomstr, WIDTH - 300, 50);
	draw_info(scene, WIDTH - 300, 70);
	draw_palette(scene, 0, 10, HEIGHT - 120);
	draw_whole_palette(scene, 0, HEIGHT - 30);
	ft_strdel(&guidestr);
	ft_strdel(&iterations);
	ft_strdel(&iterstr);
	ft_strdel(&zoom);
	ft_strdel(&zoomstr);
}
