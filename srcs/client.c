/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:01 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 22:43:36 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t	g_response[2];

// Sends all the bites of a signel character to the server
void	send_char(unsigned char c)
{
	size_t	i;
	int		ret;

	i = 0;
	while (i < sizeof(unsigned char) * 8)
	{
		//write(1, "bit\n", 4);
		ret = 0;
		if ((c >> i) & 1)
			ret = kill(g_response[0], SIGUSR1);
		else
			ret = kill(g_response[0], SIGUSR2);
		if (ret != 0)
		{
			ft_dprintf(STDERR_FILENO,
				"[MT ERROR] Failed to communicate with server");
			exit(EXIT_FAILURE);
		}
		++i;
		while (g_response[1] == 0)
			pause();
		g_response[1] = 0;
	}
}

// Handler function for SIGUSR1
// Server sends back a confirmation message once it received a bit
void	receive_server_confirmation(int signal, siginfo_t *info, void *other)
{
	(void)other;
	if (signal == SIGUSR1 && info->si_pid == g_response[0]) // can i check if info->si_pid is that of the server??
		g_response[1] = 1;
}

// Sends signals in a loop to the server until it reaches the end of the message
int	main(int ac, char **av)
{
	size_t				i;
	struct sigaction	action;

	if (ac != 3)
		return (ft_printf(MT_USAGE_CLIENT, av[0]), 0);
	if (sigemptyset(&action.sa_mask) == -1)
		return (ft_dprintf(STDERR_FILENO,
			"[MT ERROR] Failed to initialize sigaction\n"), 0);
	action.sa_sigaction = &receive_server_confirmation;
	action.sa_flags = SA_SIGINFO;
	action.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(STDERR_FILENO,
			"[MT ERROR] Failed to set SIGUSR1 handle\n"), 0);
	g_response[0] = ft_atoi(av[1]);
	if (g_response[0] < 1)
		return (ft_dprintf(2, "[MT ERROR] Invalid pid\n"), 0);
	i = 0;
	while (av[2][i])
		send_char(av[2][i++]);
	send_char('\0');
	ft_printf("[MT] Server finished receiving message\n");
	return (0);
}
