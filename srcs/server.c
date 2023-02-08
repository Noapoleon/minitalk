/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/08 11:15:35 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	main(int ac, char **av)
{
	const pid_t	pid = getpid();

	if (ac != 1)
		return (ft_printf(USAGE_SERVER, av[0]), 0);
	ft_printf("Server pid: %d\n", pid);
	return (0);
}
