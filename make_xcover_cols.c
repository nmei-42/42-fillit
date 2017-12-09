/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_xcover_cols.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 19:15:26 by nmei              #+#    #+#             */
/*   Updated: 2017/12/07 19:31:24 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"
#define ENCODE(x) (((x) < 10) ? (x) + '0' : (x) + 'a' - 10)

static void	add_tetnames(char **tmp, t_tetlst *pieces)
{
	t_tetlst	*curr_piece;

	curr_piece = pieces;
	while (curr_piece)
	{
		*(*tmp)++ = curr_piece->name[0];
		*(*tmp)++ = ' ';
		curr_piece = curr_piece->next;
	}
	*(*tmp)++ = '|';
	*(*tmp)++ = ' ';
}

/*
**	create_col_names - Function to create column names (secondary restrictions)
**	for each of the restrictions in our exact cover problem. In the case of
**	fillit our restrictions are positions in a square board (secondary) and
**	presence of a given piece (primary).
**
**	We already have names for all the pieces input into fillit so we just need
**	add them and then to generate coordinates for a given square size.
**
**	We'll use Knuth's system of representing coords as x1y1 x2y2 x3y3 so for a
**	3x3 grid we would generate: '00 01 02 10 11 12 20 21 22'.
**
**	For even larger grids we'll have a-z represent 11 through 36 so the coord
**	'a1' would (11, 2) and the coordinate 'zz' would be (36, 36).
**
**	A square length of n would mean n^2 pairs of characters and (n^2) - 1 spaces
**	we also need to allocate a string terminator so our final string length
**	will be 2 * (n^2) + (n^2) - 1 + 1 characters.
*/

char		*make_col_names(int slen, t_tetlst *pieces, ssize_t tlst_len)
{
	int		sq_col_size;
	int		row;
	int		col;
	char	*col_string;
	char	*tmp;

	sq_col_size = (2 * slen * slen + slen * slen);
	col_string = (char *)malloc(sq_col_size + (2 * (tlst_len + 1)));
	tmp = col_string;
	add_tetnames(&tmp, pieces);
	row = -1;
	while (++row < slen)
	{
		col = -1;
		while (++col < slen)
		{
			*tmp++ = ENCODE(row);
			*tmp++ = ENCODE(col);
			if (row == slen - 1 && col == slen - 1)
				*tmp = '\0';
			else
				*tmp++ = ' ';
		}
	}
	return (col_string);
}

static void	col_init(t_col *curr_col, t_col *prev_col, char *name, int primary)
{
	mod_strcpy(curr_col->name, name);
	curr_col->head.up = &(curr_col->head);
	curr_col->head.down = &(curr_col->head);
	curr_col->len = 0;
	if (primary)
	{
		curr_col->prev = prev_col;
		prev_col->next = curr_col;
	}
	else
	{
		curr_col->prev = curr_col;
		curr_col->next = curr_col;
	}
}

/*
**	link_columns - Function to link together the column names we've generated
**	such that they form a doubly linked (toroidal) list.
**
**	Note: Only primary columns are doubly linked while secondary columns
**	are linked only to themselves.
**
**	col_names takes the form of "Name1 Name2 Name3 | 00 01 02 03..." where
**	'Name's before the '|' designate primary columns and names after, designate
**	secondary columns.
*/

void		link_columns(t_xcover *xcov)
{
	t_col	*curr_col;
	char	*col_names;
	int		primary;

	primary = 1;
	col_names = xcov->col_names;
	curr_col = (xcov->col_arr + 1);
	while (*col_names)
	{
		while (*col_names == ' ')
			col_names++;
		if (*col_names == '|')
		{
			primary = 0;
			(curr_col - 1)->next = xcov->root;
			(xcov->root)->prev = (curr_col - 1);
			col_names++;
			continue;
		}
		col_init(curr_col, (curr_col - 1), col_names, primary);
		while (*col_names && *col_names != ' ')
			col_names++;
		curr_col++;
	}
}
