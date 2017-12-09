/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_xcover_rows.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 08:55:34 by nmei              #+#    #+#             */
/*   Updated: 2017/12/08 18:09:21 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"

/*
**	Bring a piece's coordinates to the top left corner
**	Will update the max row, max col, min row, and min col to reflect the new
**	coordinates.
*/

static void	norm_piece(t_tetlst *piece)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		piece->row_coords[i] = piece->row_coords[i] - piece->min_row;
		piece->col_coords[i] = piece->col_coords[i] - piece->min_col;
		i++;
	}
	piece->max_row = piece->max_row - piece->min_row;
	piece->max_col = piece->max_col - piece->min_col;
	piece->min_row = 0;
	piece->min_col = 0;
}

/*
**	Translate the 4 tetrinimo coordinates through all possible locations
**	in a square board with side length 'bsize'.
**
**	We can save time by realizing that we should only keep adding to rows and
**	columns only if they are smaller than the 'side length' - 'max length'
*/

static void	gen_row_node_names(t_tetlst *piece, t_nnlst **nnlst, int bsize)
{
	int row;
	int	col;

	row = -1;
	while (++row < bsize - piece->max_row)
	{
		col = -1;
		while (++col < bsize - piece->max_col)
		{
			nn_lstadd(nnlst, nn_lstnew(piece, row, col));
		}
	}
}

/*
**	make_node_namelst - Function to create a list of row node names that will
**	be used to link a given node to a column. Each element in the list
**	describes 1) a piece (via name) and 2) 4 coordinates for each of the four
**	blocks that make up the piece. Thus each element in the node_namelist
**	describes a possible location for a given tetrinimo piece.
*/

void		make_node_namelst(t_nnlst **nnlst, t_tetlst *pieces, int bsize)
{
	t_tetlst	*curr_p;

	curr_p = pieces;
	while (curr_p)
	{
		norm_piece(curr_p);
		gen_row_node_names(curr_p, nnlst, bsize);
		curr_p = curr_p->next;
	}
	nn_lstrev(nnlst);
}

static void	row_init(char *row, t_node *cnode, t_col *ccol, t_node **row_start)
{
	char	col_name[3];

	while (*row)
	{
		while (*row == ' ')
			row++;
		mod_strcpy(col_name, row);
		while (mod_strcmp(ccol->name, col_name))
			ccol++;
		while (*row && *row != ' ')
			row++;
		if (!(*row_start))
			*row_start = cnode;
		else
		{
			cnode->left = cnode - 1;
			(cnode - 1)->right = cnode;
		}
		cnode->col = ccol;
		cnode->up = ccol->head.up;
		ccol->head.up->down = cnode;
		ccol->head.up = cnode;
		(cnode++)->down = &(ccol->head);
		ccol->len++;
	}
}

/*
**	link_rows - Function to link our row nodes (described by the node namelst)
**	with columns. Nodes in a given row will loop back to their "row start" node.
**
**	We use the created node name list (nnlst) to determine which nodes get
**	linked to which columns/other nodes.
*/

void		link_rows(t_xcover *xcov)
{
	t_node	*cnode;
	t_node	*row_start;
	t_nnlst	*curr_row;

	cnode = xcov->node_arr;
	curr_row = xcov->nnlst;
	while (curr_row)
	{
		row_start = NULL;
		row_init(curr_row->row, cnode, xcov->col_arr, &row_start);
		cnode += 5;
		row_start->left = cnode - 1;
		(cnode - 1)->right = row_start;
		curr_row = curr_row->next;
	}
}
