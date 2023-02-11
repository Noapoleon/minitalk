/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 09:21:36 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/11 13:35:07 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include "libft.h"
# include <unistd.h>
# include <signal.h>
# include <stdio.h>

# define MT_USAGE_SERVER	"Usage: %s"
# define MT_USAGE_CLIENT	"Usage: %s <Server pid> <Message>"
# define MT_BUFSIZE	512

typedef struct s_client_list	t_client_list;
typedef struct s_msgbuf_list	t_msgbuf_list;
struct	s_msgbuf_list
{
	char			data[MT_BUFSIZE];
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
// client_list2.c
t_msgbuf_list	*client_msgbuf_new(t_client_list *client);

#endif
