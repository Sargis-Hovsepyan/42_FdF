/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 17:36:19 by shovsepy          #+#    #+#             */
/*   Updated: 2021/07/09 18:32:05 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	fdf_usage(void)
{
	ft_putstr_fd("Usage:\n\t", 1);
	ft_putstr_fd("FDF", 1);
	ft_putstr_fd("fdf_file\n", 1);
	ft_putstr_fd("Controls:\n", 1);
	ft_putstr_fd("\t[Esc]        -> Exit the program.\n", 1);
	ft_putstr_fd("\t[R]          -> Reset the map.\n", 1);
	ft_putstr_fd("\t[M]          -> Colorize this shit!.\n", 1);
	ft_putstr_fd("\t[B][N]       -> Zoom in and zoom out.\n", 1);
	ft_putstr_fd("\t[J][K]       -> Spiky time!\n", 1);
	ft_putstr_fd("\t[Space]      -> Change the camera(isometric <-> plain).\n", 1);
	ft_putstr_fd("\t[W][A][S][D] -> Move the map.\n", 1);
}

static void	reset_map(t_fdf *fdf)
{
	fdf->map.coordinate_z = 0;
	fdf->map.coordinate_y = 0;
	fdf->map.x_value = 1.00;
	fdf->map.angle_y = cos(M_PI / 3);
	fdf->map.angle_z = fdf->map.angle_y * sin(M_PI / 6);
	fdf->map.zoom = ceil((fdf->map.width > fdf->map.height)) \
		? (WIN_WIDTH / fdf->map.width) + MAGIC_ZOOM \
		: (WIN_HEIGHT / fdf->map.height) + MAGIC_ZOOM;
	fdf->map.isometric = 1;
	fdf->color.red = 0x4F;
	fdf->color.green = 0x4F;
	fdf->color.blue = 0x4F;
}

static void	random_color(t_fdf *fdf)
{
	fdf->color.red = (rand() % 0x7F);
	fdf->color.green = (rand() % 0x7F);
	fdf->color.blue = (rand() % 0x7F);
}

static int	fdf_keys(int keycode, t_fdf *fdf)
{
	if (keycode == KEY_ESCAPE)
		exit(0);
	else if (keycode == KEY_ANSI_R)
		reset_map(fdf);
	else if (keycode == KEY_ANSI_M)
		random_color(fdf);
	else if (keycode == KEY_ANSI_W || keycode == KEY_UPARROW)
		fdf->map.coordinate_z -= 1;
	else if (keycode == KEY_ANSI_S || keycode == KEY_DOWNARROW)
		fdf->map.coordinate_z += 1;
	else if (keycode == KEY_ANSI_D || keycode == KEY_RIGHTARROW)
		fdf->map.coordinate_y -= 1;
	else if (keycode == KEY_ANSI_A || keycode == KEY_LEFTARROW)
		fdf->map.coordinate_y += 1;
	else if (keycode == KEY_ANSI_B)
		fdf->map.zoom += 1;
	else if ((keycode == KEY_ANSI_N) && (fdf->map.zoom > MAX_ZOOM))
		fdf->map.zoom -= 1;
	else if ((keycode == KEY_ANSI_J) && (fdf->map.x_value < MAX_X))
		fdf->map.x_value += 0.25;
	else if ((keycode == KEY_ANSI_K) && (fdf->map.x_value > -MAX_X))
		fdf->map.x_value -= 0.25;
	else if (keycode == KEY_SPACE)
		fdf->map.angle_z *= (fdf->map.isometric++ % 2) ? 0.2 : 5;
	return (0);
}

int	main(int argc, char **argv)
{
	t_fdf	*fdf;

	if (argc == 2)
	{
		fdf = (t_fdf *)malloc(sizeof(t_fdf));
		if (!fdf)
			ft_error("Memory allocation failed", 3);
		ft_read(argv[1], fdf);
		reset_map(fdf);
		fdf->mlx.init = mlx_init();
		fdf->mlx.win = mlx_new_window(fdf->mlx.init, WIN_WIDTH, WIN_HEIGHT, TITLE(argv[1]));
		mlx_hook(fdf->mlx.win, 2, 3, fdf_keys, fdf);
		mlx_loop_hook(fdf->mlx.init, ft_draw, fdf);
		mlx_loop(fdf->mlx.init);
	}
	else
		fdf_usage();
	return (0);
}