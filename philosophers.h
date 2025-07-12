/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 20:33:57 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/07/11 23:22:43 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					left_fork;
	int					right_fork;
	int					eat_count;
	pthread_mutex_t		count_mutex;
	long long			last_eat;
	pthread_t			thread;
	pthread_mutex_t		mutex;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int					philo_count;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	int					stop;
	long long			start;
	t_philo				*philos;
	pthread_mutex_t		*fork_locks;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		dead_lock;
}						t_data;

int						is_valid_args(int argc, char **argv);
int						init_data(t_data *data, int argc, char **argv);
int						init_mutexes(t_data *data);
int						init_philosophers(t_data *data);

int						start_simulation(t_data *data);
void					*philo_routine(void *arg);
void					philo_eat(t_philo *philo);

void					print_status(t_philo *philo, char *msg, int death);
void					free_all(t_data *data);
int						ft_atoi(const char *str);
long long				get_time_ms(void);
void					ft_usleep(long long time, t_data *data);

void					monitor_philos(t_data *data);

#endif