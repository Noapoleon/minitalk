/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/09 22:39:54 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	receive_char(int signal, siginfo_t *info, void *other) // look up what other is for here
{
	static char				message[2048]; // hard limited not protected yet; can also make this a array of some sort of strut that keeps every client's message status for multiple client sending messages at the same time
	static unsigned char	character;
	static int				index;
	static size_t			shift;
	char					bit;

	(void)other;
	if (signal == SIGUSR1)
		bit = 1;
	else if (signal == SIGUSR2)
		bit = 0;
	else // REMOVE LATER, THIS FOR DEBUG
		ft_printf("WE GOT A PROBLEM HUSTON!\n");
	//character |= bit << ((sizeof(unsigned char) * 8 - 1) - shift);
	character  |= bit << shift;
	++shift;
	if (shift == 8)
	{
		shift = 0;
		message[index++] = character;
		if (character == '\0')
		{
			ft_putstr_fd(message, 1);
			index = 0;
		}
		character = 0;
	}
	//ft_printf("about to send message to client\n");
	if (kill(info->si_pid, SIGUSR1) == -1)
	{
		perror("[MINITALK] kill SIGUSR1");
		exit(EXIT_FAILURE);
	}
	//ft_printf("sent a signal to client of pid %d\n", info->si_pid);
}

int	main(int ac, char **av)
{
	struct sigaction	action;

	if (ac != 1)
		return (ft_printf(USAGE_SERVER, av[0]), 0);
	ft_printf("Server pid: %d\n", getpid());
	if (sigemptyset(&action.sa_mask) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to initialize sigaction.\n"), 0);
	action.sa_sigaction = &receive_char;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	while (1)
		pause();
	return (0);
}
