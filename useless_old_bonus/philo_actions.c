/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 18:40:41 by mframbou          #+#    #+#             */
/*   Updated: 01-02-2022 16:49 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_for_forks(t_philosopher *philo)
{
	sem_wait(philo->sem);
}

// Init the first meal at -(time to eat), so that first eat will be set at 0
// and so on (last meal time is set to the time he began to eat)
int	philo_eat(t_philosopher *philo)
{
	long	current_ms;

	sem_wait(philo->sem);
	if (!lock_forks(philo))
		while (get_ms_since_start() <= philo->timings->time_to_die)
			usleep(50);
	current_ms = get_ms_since_start();
	if (!check_death(philo, current_ms))
		return (-1);
	printf("%ld %d is eating\n", current_ms, philo->number);
	philo->last_meal_end += philo->timings->time_to_eat;
	if (philo->times_ate == 0)
		philo->last_meal_end = current_ms + philo->timings->time_to_eat;
	philo->times_ate++;
	while (current_ms < philo->last_meal_end)
	{
		if (!check_death(philo, current_ms))
			return (-1);
		usleep(50);
		current_ms = get_ms_since_start();
	}
	sem_post(philo->sem);
	return (0);
}

// If last meal was at 0 and philo took 200 to eat, 100 to sleep,
// then we should wait till 300 ms
int	philo_sleep(t_philosopher *philo)
{
	long	current_ms;

	if (*(philo->someone_died))
		return (-1);
	current_ms = get_ms_since_start();
	printf("%ld %d is sleeping\n", current_ms, philo->number);
	while (current_ms < philo->last_meal_end + philo->timings->time_to_sleep)
	{
		if (!check_death(philo, current_ms))
			return (-1);
		usleep(50);
		current_ms = get_ms_since_start();
	}
	return (0);
}

int	philo_think(t_philosopher *philo)
{
	if (*(philo->someone_died))
		return (-1);
	printf("%ld %d is thinking\n", get_ms_since_start(), philo->number);
	return (0);
}