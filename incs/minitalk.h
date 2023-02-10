/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:21:36 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/10 16:40:50 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include "libft.h"
# include <unistd.h>
# include <signal.h>
# include <stdio.h>

# define USAGE_SERVER	"Usage: %s"
# define USAGE_CLIENT	"Usage: %s <Server pid> <Message>"
# define MAX_MSG_SIZE	512

typedef struct s_client_list	t_client_list;
typedef struct s_msgbuf_list	t_msgbuf_list;
struct	s_msgbuf_list
{
	char			data[MAX_MSG_SIZE];
	t_msgbuf_list	*next;
};
struct	s_client_list
{
	pid_t			pid;
	t_msgbuf_list	*msg;
	t_msgbuf_list	*last_buf;
	int				index;
	unsigned char	byte;
	size_t			shift;
	t_client_list	*next;
};

// client_list.c
t_client_list	*client_new_front(t_client_list **begin, const pid_t pid);
t_client_list	*client_get_pid(const t_client_list *begin, const pid_t pid);
t_client_list	*client_get(t_client_list **begin, const pid_t pid);
void			client_msg_remove(t_client_list *client);
void			client_remove(t_client_list **begin, t_client_list *remove);
t_msgbuf_list	*client_msgbuf_new(t_client_list *client);

#endif
