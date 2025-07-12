/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:02:32 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/07/12 00:12:50 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

static void	monitor_philos_util(t_data *data, int *full_philos)
{
	int			i;
	long long	now;

	i = -1;
	while (++i < data->philo_count)
	{
		pthread_mutex_lock(&data->dead_lock);
		if (data->must_eat_count > 0
			&& data->philos[i].eat_count >= data->must_eat_count)
		{
			(*full_philos)++;
			pthread_mutex_unlock(&data->dead_lock);
			continue ;
		}
		now = get_time_ms();
		if (now - data->philos[i].last_eat > data->time_to_die && !data->stop)
		{
			data->stop = 1;
			pthread_mutex_unlock(&data->dead_lock);
			print_status(&data->philos[i], "died", 1);
			return ;
		}
		pthread_mutex_unlock(&data->dead_lock);
	}
}

void	monitor_philos(t_data *data)
{
	int	full_philos;

	while (1)
	{
		pthread_mutex_lock(&data->dead_lock);
		if (data->stop)
		{
			pthread_mutex_unlock(&data->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&data->dead_lock);
		full_philos = 0;
		monitor_philos_util(data, &full_philos);
		if (data->must_eat_count > 0 && full_philos == data->philo_count)
		{
			pthread_mutex_lock(&data->dead_lock);
			data->stop = 1;
			pthread_mutex_unlock(&data->dead_lock);
			break ;
		}
		usleep(10);
	}
}

void	print_status(t_philo *philo, char *msg, int death)
{
	t_data	*data;
	int		stop;

	data = philo->data;
	pthread_mutex_lock(&data->dead_lock);
	stop = data->stop;
	if (stop && !death)
	{
		pthread_mutex_unlock(&data->dead_lock);
		return ;
	}
	pthread_mutex_lock(&data->write_lock);
	pthread_mutex_unlock(&data->dead_lock);
	printf("%lld %d %s\n", get_time_ms() - data->start, philo->id, msg);
	pthread_mutex_unlock(&data->write_lock);
}
