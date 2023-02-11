/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:22:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/11 13:12:14 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// Loops through the message chained list and outputs it
void	write_message(const t_msgbuf_list *msg)
{
	while (msg != NULL && msg->next != NULL)
	{
		write(STDOUT_FILENO, msg->data, MT_BUFSIZE);
		msg = msg->next;
	}
	ft_putstr_fd((char *)msg->data, STDOUT_FILENO);
}

// Adds a character to the latest buffer and allocates a new one if the current
// one is full
void	save_char(t_client_list **begin, t_client_list *client)
{
	client->shift = 0;
	client->last_buf->data[(client->index)++] = client->byte;
	if (client->byte == '\0')
	{
		write_message(client->msg);
		client_remove(begin, client);
	}
	else
	{
		if (client->index == MT_BUFSIZE)
		{
			if (client_msgbuf_new(client) == NULL)
			{
				ft_dprintf(STDERR_FILENO,
					"[MT ERROR] Failed to make new message buffer\n");
				exit(EXIT_FAILURE);
			}
			client->index = 0;
		}
		client->byte = 0;
	}
}

// Handle function for SIGUSR1 and SIGUSR2
void	receive_bit(int signal, siginfo_t *info, void *context)
{
	static t_client_list	*clients;
	t_client_list			*curr;
	unsigned char			bit;

	(void)context;
	curr = client_get(&clients, info->si_pid);
	if (curr == NULL)
	{
		ft_dprintf(STDERR_FILENO,
			"[MT ERROR] Failed to make new client entry\n");
		exit(EXIT_FAILURE);
	}
	if (signal != SIGUSR1 && signal != SIGUSR2)
		return ;
	bit = (signal == SIGUSR1);
	curr->byte |= (bit << (curr->shift)++);
	if (curr->shift == sizeof(unsigned char) * 8)
		save_char(&clients, curr);
	if (kill(info->si_pid, SIGUSR1) == -1)
	{
		ft_dprintf(STDERR_FILENO,
			"[MT ERROR] Failed to send confirmation to client of pid %d\n",
			info->si_pid);
		exit(EXIT_FAILURE);
	}
}

// Receives signals that interrupt the while pause loop
int	main(int ac, char **av)
{
	struct sigaction	action;

	if (ac != 1)
		return (ft_printf(MT_USAGE_SERVER, av[0]), 0);
	ft_printf("Server pid: %d\n", getpid());
	if (sigemptyset(&action.sa_mask) == -1)
		return (ft_dprintf(2,
			"[MT ERROR] Failed to initialize sigaction\n"), 0);
	action.sa_flags = SA_SIGINFO;
	action.sa_flags |= SA_RESTART;
	action.sa_sigaction = &receive_bit;
	if (sigaction(SIGUSR1, &action, NULL) == -1)
		return (ft_dprintf(2, "[MT ERROR] Failed to set SIGUSR1 handle\n"), 0);
	if (sigaction(SIGUSR2, &action, NULL) == -1)
		return (ft_dprintf(2, "[MT ERROR] Failed to set SIGUSR2 handle\n"), 0);
	while (1)
		pause();
	return (0);
}
