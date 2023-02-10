/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 18:56:43 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	reset_char(char *message, unsigned char *character, int *index,
	size_t *shift)
{
	if (*shift == 8)
	{
		*shift = 0;
		message[(*index)++] = *character;
		if (*character == '\0')
		{
			ft_putstr_fd(message, 1);
			*index = 0;
		}
		else if (*index == MAX_MSG_SIZE)
		{
			write(1, message, MAX_MSG_SIZE);
			*index = 0;
		}
		*character = 0;
	}
}

void	receive_bit(int signal, siginfo_t *info, void *other) // look up what other is for here
{
	static char				message[MAX_MSG_SIZE]; // hard limited not protected yet; can also make this a array of some sort of strut that keeps every client's message status for multiple client sending messages at the same time
	static unsigned char	character;
	static int				index;
	static size_t			shift;
	char					bit;

	(void)other;
	if (signal != SIGUSR1 && signal != SIGUSR2)
		return ;
	bit = (signal == SIGUSR1);
	character |= bit << shift++;
	reset_char(message, &character, &index, &shift);
	if (kill(info->si_pid, SIGUSR1) == -1)
	{
		perror("[MINITALK] kill SIGUSR1");
		exit(EXIT_FAILURE);
	}
}

int	main(int ac, char **av)
{
	struct sigaction	action;

	if (ac != 1)
		return (ft_printf(USAGE_SERVER, av[0]), 0);
	ft_printf("Server pid: %d\n", getpid());
	if (sigemptyset(&action.sa_mask) == -1)
		return (ft_dprintf(STDERR_FILENO,
			"[ERROR] Failed to initialize sigaction.\n"), 0);
	action.sa_sigaction = &receive_bit;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(2,
			"[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (ft_dprintf(2,
			"[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	while (1)
		pause();
	return (0);
}
