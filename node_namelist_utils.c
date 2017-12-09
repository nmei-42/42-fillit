/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_namelist_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 09:55:52 by nmei              #+#    #+#             */
/*   Updated: 2017/12/08 16:38:34 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"
#define ENCODE(x) (((x) < 10) ? (x) + '0' : (x) + 'a' - 10)

t_nnlst		*nn_lstnew(t_tetlst *piece, int row, int col)
{
	t_nnlst *new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if ((new = (t_nnlst *)malloc(sizeof(*new))) == NULL)
		return (NULL);
	new->row[j++] = piece->name[0];
	new->row[j++] = ' ';
	while (i < 4)
	{
		new->row[j++] = ENCODE(piece->row_coords[i] + row);
		new->row[j++] = ENCODE(piece->col_coords[i] + col);
		if (i != 3)
			new->row[j++] = ' ';
		i++;
	}
	new->row[j] = '\0';
	new->next = NULL;
	return (new);
}

void		nn_lstadd(t_nnlst **alst, t_nnlst *new)
{
	if (alst && new)
	{
		if (*alst)
			new->next = *alst;
		*alst = new;
	}
}

size_t		nn_lstlen(t_nnlst *begin_list)
{
	size_t	count;

	count = 0;
	while (begin_list)
	{
		begin_list = begin_list->next;
		count++;
	}
	return (count);
}

void		nn_lstrev(t_nnlst **begin_list)
{
	t_nnlst *new_begin;
	t_nnlst *temp_next;

	if (begin_list && nn_lstlen(*begin_list) > 1)
	{
		new_begin = NULL;
		while (*begin_list)
		{
			temp_next = (*begin_list)->next;
			(*begin_list)->next = new_begin;
			new_begin = *begin_list;
			*begin_list = temp_next;
		}
		*begin_list = new_begin;
	}
}

void		nn_lstfree(t_nnlst **begin_list)
{
	t_nnlst	*curr_elem;
	t_nnlst *next_elem;

	if (begin_list)
	{
		curr_elem = *begin_list;
		while (curr_elem)
		{
			next_elem = curr_elem->next;
			free(curr_elem);
			curr_elem = next_elem;
		}
		*begin_list = NULL;
	}
}
