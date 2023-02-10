/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:01 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 18:52:57 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t	g_response[2];

void	send_char(unsigned char c, pid_t pid_server)
{
	size_t	i;
	int		ret;

	i = 0;
	while (i < sizeof(unsigned char) * 8)
	{
		ret = 0;
		if ((c >> i) & 1)
			ret = kill(pid_server, SIGUSR1);
		else
			ret = kill(pid_server, SIGUSR2);
		if (ret != 0)
		{
				perror("[MINITALK] send_char > kill");
				exit(EXIT_FAILURE);
		}
		++i;
		while (g_response[1] == 0)
			pause();
		g_response[1] = 0;
	}
}

void	receive_server_confirmation(int signal, siginfo_t *info, void *other)
{
	(void)other;
	if (signal == SIGUSR1 && info->si_pid == g_response[0]) // can i check if info->si_pid is that of the server??
		g_response[1] = 1;
}

int	main(int ac, char **av)
{
	size_t			i;
	struct sigaction	action;

	if (ac != 3)
		return (ft_printf(USAGE_CLIENT, av[0]), 0);
	if (sigemptyset(&action.sa_mask) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to initialize sigaction.\n"), 0);
	action.sa_sigaction = &receive_server_confirmation;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	g_response[0] = ft_atoi(av[1]);
	i = 0;
	while (av[2][i])
		send_char(av[2][i++], g_response[0]);
	send_char('\0', g_response[0]);
	ft_printf("Server finished receiving message.\n");
	return (0);
}
