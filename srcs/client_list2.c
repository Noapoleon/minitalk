/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_list2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 21:04:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 21:35:01 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// Allocates a t_msgbuf_list and puts it at the end of the client msg list
// Also sets variables and changes the last_buf pointer to the newly created one
t_msgbuf_list	*client_msgbuf_new(t_client_list *client)
{
	client->last_buf->next = malloc(sizeof(t_msgbuf_list));
	if (client->last_buf->next == NULL)
		return (NULL);
	client->last_buf->next->next = NULL;
	client->last_buf = client->last_buf->next;
	return (client->last_buf);
}
