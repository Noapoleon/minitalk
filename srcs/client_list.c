/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 13:20:38 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 16:48:40 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

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

t_msgbuf_list	*client_msgbuf_new(t_client_list *client)
{
	client->last_buf->next = malloc(sizeof(t_msgbuf_list));
	if (client->last_buf->next == NULL)
		return (NULL);
	client->last_buf->next->next = NULL;
	client->last_buf = client->last_buf->next;
	return (client->last_buf);
}

