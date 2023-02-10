/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 16:40:27 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	write_message(const t_msgbuf_list	*msg)
{
	while (msg != NULL && msg->next != NULL)
	{
		write(1, msg->data, MAX_MSG_SIZE);
		msg = msg->next;
	}
	ft_putstr_fd((char *)msg->data, 1);
}

void	receive_bit(int signal, siginfo_t *info, void *other) // look up what other is for here
{
	static t_client_list	*clients;
	t_client_list			*curr;
	unsigned char			bit;

	curr = client_get(&clients, info->si_pid);
	if (curr == NULL)
	{
		ft_dprintf(STDERR_FILENO,
			"[MINITALK] Failed to make new client entry\n");
		exit(EXIT_FAILURE);
	}
	(void)other;
	if (signal == SIGUSR1)
		bit = 1;
	else if (signal == SIGUSR2)
		bit = 0;
	else // REMOVE LATER, THIS FOR DEBUG
	{
		ft_printf("HUSTON WE GOT A PROBLEM, NON HANDLED SIGNAL!!\n");
		ft_printf("Signal -> %d\n", signal);
		exit(EXIT_FAILURE); // this will cause client list leaks
	}
	curr->byte |= bit << curr->shift++;
	if (curr->shift == 8)
	{
		curr->shift = 0;
		curr->last_buf->data[curr->index++] = curr->byte;
		if (curr->byte == '\0')
		{
			write_message(curr->msg);
			client_remove(&clients, curr);
		}
		else
		{
			if (curr->index == MAX_MSG_SIZE && client_msgbuf_new(curr) == NULL)
			{
				ft_dprintf(STDERR_FILENO,
					"[MINITALK] Failed to make new message buffer\n");
				exit(EXIT_FAILURE);
			}
			curr->byte = 0;
		}
	}
	if (kill(info->si_pid, SIGUSR1) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			"[MINITALK] Failed to send confirmation to client %d",
			info->si_pid);
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
		return (ft_dprintf(2, "[ERROR] Failed to initialize sigaction.\n"), 0);
	action.sa_sigaction = &receive_bit;
	action.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to set SIGUSR1 handle.\n"), 0);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (ft_dprintf(2, "[ERROR] Failed to set SIGUSR2 handle.\n"), 0);
	while (1)
		pause();
	return (0);
}
