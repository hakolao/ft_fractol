/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 13:59:45 by ohakola           #+#    #+#             */
/*   Updated: 2020/02/20 16:01:08 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void		mlx_init_scene(t_scene *scene)
{
	mlx_hook(scene->mlx_wdw, 2, 0, handle_key_press, scene);
	mlx_hook(scene->mlx_wdw, 3, 0, handle_key_release, scene);
	mlx_hook(scene->mlx_wdw, 4, 0, handle_mouse_button_press, scene);
	mlx_hook(scene->mlx_wdw, 5, 0, handle_mouse_button_release, scene);
	mlx_hook(scene->mlx_wdw, 6, 0, handle_mouse_move, scene);
	mlx_hook(scene->mlx_wdw, 17, 0, handle_exit_event, scene);
}

/*
** Separates fractals into windows depending on input
** fractal array (see enum e_fractal) and its size
*/

int				init_fractol(int *artists, int size)
{
	t_scenes	*data;
	int			i;

	if (!(data = malloc(sizeof(*data))) ||
		!(data->scenes = malloc(sizeof(*data->scenes) * size)) ||
		!(data->mlx = mlx_init()))
		return (FALSE);
	data->size = size;
	i = -1;
	while (++i < size)
	{
		if (!(data->scenes[i] =
				new_scene(data->mlx, artists[i], WIDTH, HEIGHT)) &&
			log_err("Failed to create scene.", strerror(5)))
			return (FALSE);
		data->scenes[i]->id = i;
		data->scenes[i]->data = data;
		mlx_init_scene(data->scenes[i]);
	}
	mlx_loop_hook(data->mlx, handle_loop, data);
	mlx_loop(data->mlx);
	return (FALSE);
}

int				parse_args(int argc, char **argv)
{
	int	i;
	int	fractals[argc - 1];
	int	size;

	i = -1;
	size = 0;
	while (++i < argc - 1)
	{
		fractals[i] = -1;
		if (ft_strequ(argv[i + 1], "all"))
		{
			return (init_fractol((int[9]){mandelbrot, julia, burning_ship,
				mandelbrot_n, julia_n, bird_of_prey, julia_mod,
				phoenix_mod, burning_julia}, 9));
		}
		if (!check_args(argv[i + 1], &size, &fractals[i]))
			return (log_guide() && FALSE);
	}
	if (size > MAX_WINDOWS)
		return (log_err("Too many windows: max 10", strerror(5)));
	ft_sort_int_tab(fractals, argc - 1, -1);
	return (init_fractol(fractals, size));
}

int				main(int argc, char **argv)
{
	if (argc > 1)
		parse_args(argc, argv);
	else
		log_guide() &&
			init_fractol(&(int){mandelbrot}, 1);
	return (0);
}
