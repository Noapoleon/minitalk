/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:01 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/09 18:31:44 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	main(int ac, char **av)
{
	pid_t			pid_server;
	size_t			i;
	size_t			j;
	unsigned char	tmp;

	if (ac != 3)
		return (ft_printf(USAGE_CLIENT, av[0]), 0);  
	pid_server = ft_atoi(av[1]);
	i = 0;
	while (av[2][i])
	{
		tmp = av[2][i];
		j = 0;
		while (j < sizeof(unsigned char) * 8)
		{
			if ((tmp >> j) & 1)
				kill(pid_server, SIGUSR1);
			else
				kill(pid_server, SIGUSR2);
			++j;
			usleep(50); // debug only
		}
		//pause();
		++i;
	}
	//kill(pid_server, SIGUSR1);
	//usleep(50);
	//kill(pid_server, SIGUSR2);
	return (0);
}
