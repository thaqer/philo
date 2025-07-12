/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initilayzation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:43:40 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/07/11 22:54:11 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	if (data->philo_count <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (data->must_eat_count <= 0 && data->must_eat_count != -1))
		return (0);
	data->stop = 0;
	return (1);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->fork_locks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->fork_locks)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->fork_locks[i], NULL) != 0)
		{
			data->philo_count = i;
			free_all(data);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&data->write_lock, NULL) != 0
		|| pthread_mutex_init(&data->dead_lock, NULL) != 0)
	{
		free_all(data);
		return (0);
	}
	return (1);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (0);
	i = -1;
	while (++i < data->philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->philo_count;
		data->philos[i].eat_count = 0;
		data->philos[i].last_eat = 0;
		data->philos[i].data = data;
		if (pthread_mutex_init(&data->philos[i].mutex, NULL)
			|| pthread_mutex_init(&data->philos[i].count_mutex, NULL) != 0)
		{
			free_all(data);
			return (0);
		}
	}
	return (1);
}
