/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 13:20:38 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 21:37:41 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// Allocates a new client struct and puts it to the front of the list
t_client_list	*client_new_front(t_client_list **begin, const pid_t pid)
{
	t_client_list	*tmp;

	if (begin == NULL)
		return (NULL);
	tmp = malloc(sizeof(t_client_list));
	if (tmp == NULL)
		return (NULL);
	tmp->pid = pid;
	tmp->last_buf = malloc(sizeof(t_msgbuf_list));
	if (tmp->last_buf == NULL)
		return (free(tmp), NULL);
	tmp->last_buf->next = NULL;
	tmp->msg = tmp->last_buf;
	tmp->index = 0;
	tmp->byte = 0;
	tmp->shift = 0;
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}

// Finds a client by it's pid in the clients list
t_client_list	*client_get_pid(const t_client_list *begin, const pid_t pid)
{
	t_client_list	*curr;

	curr = (t_client_list *)begin;
	while (curr != NULL)
	{
		if (curr->pid == pid)
			break ;
		curr = curr->next;
	}
	return (curr);
}

// Finds a client in the list based on its pid, if it doesn't exist yet it
// safely allocates a new one
t_client_list	*client_get(t_client_list **begin, const pid_t pid)
{
	t_client_list	*curr;

	curr = client_get_pid(*begin, pid);
	if (curr == NULL)
	{
		curr = client_new_front(begin, pid);
		if (curr == NULL)
			return (NULL);
	}
	return (curr);
}

// Frees a client message list
void	client_msg_remove(t_client_list *client)
{
	t_msgbuf_list	*tmp;

	while (client->msg != NULL)
	{
		tmp = client->msg;
		client->msg = client->msg->next;
		free(tmp);
	}
}

// Properly frees a client struct and its elements
void	client_remove(t_client_list **begin, t_client_list *remove)
{
	t_client_list	*prev;

	if (begin == NULL || *begin == NULL || remove == NULL)
		return ;
	if (remove == *begin)
		return ((*begin = (*begin)->next), client_msg_remove(remove),
			free(remove));
	prev = *begin;
	while (prev->next != NULL && prev->next != remove)
		prev = prev->next;
	if (prev->next == NULL)
		return ;
	prev->next = remove->next;
	client_msg_remove(remove);
	free(remove);
}
