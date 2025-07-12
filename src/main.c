/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 20:37:50 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/05/23 23:12:09 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	is_valid_args(int argc, char **argv)
{
	int	x;
	int	y;

	if (argc != 5 && argc != 6)
		return (0);
	y = 1;
	while (y < argc)
	{
		x = 0;
		while (argv[y][x])
		{
			if (argv[y][x] < '0' || argv[y][x] > '9')
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!is_valid_args(argc, argv))
		return (write(2, "Invalid arguments\n", 18), 1);
	if (!init_data(&data, argc, argv))
		return (write(2, "Invalid values\n", 15), 1);
	if (!init_mutexes(&data) || !init_philosophers(&data))
		return (free_all(&data), write(2, "Init error\n", 11), 1);
	if (!start_simulation(&data))
		return (free_all(&data), write(2, "Simulation error\n", 17), 1);
	free_all(&data);
	return (0);
}
