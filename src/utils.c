/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 05:19:09 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/07/11 23:32:27 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_atoi(const char *str)
{
	long	result;

	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str++ - '0');
		if (result > 2147483647)
			return (-1);
	}
	return ((int)result);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time, t_data *data)
{
	long long	start;
	int			stop;

	start = get_time_ms();
	while (1)
	{
		pthread_mutex_lock(&data->dead_lock);
		stop = data->stop;
		pthread_mutex_unlock(&data->dead_lock);
		if (stop || get_time_ms() - start >= time)
			break ;
		usleep(100);
	}
}

static void	destroy(t_data *data)
{
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->dead_lock);
}

void	free_all(t_data *data)
{
	int	i;

	if (data->fork_locks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->fork_locks[i]);
			i++;
		}
		free(data->fork_locks);
		data->fork_locks = NULL;
	}
	if (data->philos)
	{
		i = -1;
		while (++i < data->philo_count)
		{
			pthread_mutex_destroy(&data->philos[i].mutex);
			pthread_mutex_destroy(&data->philos[i].count_mutex);
		}
		free(data->philos);
		data->philos = NULL;
	}
	destroy(data);
}
