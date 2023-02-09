/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:01 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/09 22:57:32 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

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
		//ft_printf("sent something to server\n");
		++i;
		pause();
	}
}

void	receive_server_confirmation(int signal, siginfo_t *info, void *other)
{
	// dont know if i can make it so that it only resumes when the signal comes from the server
	// but it shouldn't even matter i think
	(void)signal;
	(void)info;
	(void)other;
}

int	main(int ac, char **av)
{
	pid_t			pid_server;
	size_t			i;
	struct sigaction	action;

	if (ac != 3)
		return (ft_printf(USAGE_CLIENT, av[0]), 0);
	//ft_printf("pid of client is %d\n", getpid());
	if (sigemptyset(&action.sa_mask) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to initialize sigaction.\n"), 0);
	action.sa_sigaction = &receive_server_confirmation;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	pid_server = ft_atoi(av[1]);
	i = 0;
	while (av[2][i])
		send_char(av[2][i++], pid_server);
	send_char('\0', pid_server);
	ft_printf("Server finished receiving message.\n");
	return (0);
}
