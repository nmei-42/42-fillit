/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tetra_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 13:32:34 by nmei              #+#    #+#             */
/*   Updated: 2017/12/08 16:38:27 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"

t_tetlst	*tet_lstnew(char name)
{
	t_tetlst *new;

	if ((new = (t_tetlst *)malloc(sizeof(*new))) == NULL)
		return (NULL);
	new->is_valid = 1;
	new->next = NULL;
	new->neighbors = 0;
	new->block_count = 0;
	new->name[0] = name;
	new->min_row = 99999;
	new->min_col = 99999;
	new->max_row = 0;
	new->max_col = 0;
	return (new);
}

void		tet_lstadd(t_tetlst **alst, t_tetlst *new)
{
	if (alst && new)
	{
		if (*alst)
			new->next = *alst;
		*alst = new;
	}
}

size_t		tet_lstlen(t_tetlst *begin_list)
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

void		tet_lstrev(t_tetlst **begin_list)
{
	t_tetlst *new_begin;
	t_tetlst *temp_next;

	if (begin_list && tet_lstlen(*begin_list) > 1)
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

void		tet_lstfree(t_tetlst **begin_list)
{
	t_tetlst	*curr_elem;
	t_tetlst	*next_elem;

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
