/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:27:24 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/07/11 23:38:36 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

static int	start_simulation_util(t_data *data)
{
	print_status(&data->philos[0], "has taken a fork", 0);
	ft_usleep(data->time_to_die, data);
	print_status(&data->philos[0], "died", 1);
	return (1);
}

int	start_simulation(t_data *data)
{
	int	i;
	int	j;

	data->start = get_time_ms();
	if (data->philo_count == 1)
		return (start_simulation_util(data));
	i = -1;
	while (++i < data->philo_count)
	{
		data->philos[i].last_eat = data->start;
		data->philos[i].eat_count = 0;
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]) != 0)
		{
			j = 0;
			while (j < i)
				pthread_join(data->philos[j++].thread, NULL);
			return (0);
		}
	}
	monitor_philos(data);
	i = -1;
	while (++i < data->philo_count)
		pthread_join(data->philos[i].thread, NULL);
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		stop;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		usleep(200);
	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_lock);
		stop = philo->data->stop;
		pthread_mutex_unlock(&philo->data->dead_lock);
		if (stop)
			break ;
		philo_eat(philo);
		if (philo->data->must_eat_count > 0
			&& philo->eat_count >= philo->data->must_eat_count)
			break ;
		print_status(philo, "is sleeping", 0);
		ft_usleep(philo->data->time_to_sleep, philo->data);
		print_status(philo, "is thinking", 0);
		usleep(100);
	}
	return (NULL);
}

static void	philo_eat_utils(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->dead_lock);
	philo->last_eat = get_time_ms();
	pthread_mutex_unlock(&data->dead_lock);
	print_status(philo, "is eating", 0);
	ft_usleep(data->time_to_eat, data);
	pthread_mutex_lock(&data->dead_lock);
	philo->eat_count++;
	pthread_mutex_unlock(&data->dead_lock);
}

void	philo_eat(t_philo *philo)
{
	t_data	*data;
	int		left;
	int		right;

	data = philo->data;
	left = philo->left_fork;
	right = philo->right_fork;
	if (left < right)
	{
		pthread_mutex_lock(&data->fork_locks[left]);
		print_status(philo, "has taken a fork", 0);
		pthread_mutex_lock(&data->fork_locks[right]);
		print_status(philo, "has taken a fork", 0);
	}
	else
	{
		pthread_mutex_lock(&data->fork_locks[right]);
		print_status(philo, "has taken a fork", 0);
		pthread_mutex_lock(&data->fork_locks[left]);
		print_status(philo, "has taken a fork", 0);
	}
	philo_eat_utils(philo, data);
	pthread_mutex_unlock(&data->fork_locks[left]);
	pthread_mutex_unlock(&data->fork_locks[right]);
}
