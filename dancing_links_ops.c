/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dancing_links_ops.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 16:25:24 by nmei              #+#    #+#             */
/*   Updated: 2017/12/06 15:18:19 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
**	Implementation of Donald Knuth's DLX algorithm
**	https://arxiv.org/pdf/cs/0011047.pdf
**
**	Read the cover_col() function first!
*/

/*
**	Function to "cover" all nodes in the same row as a given (col_node)
**	1) Move to the node (row_node) just right of the (col_node)
**	2) assign (row_node)'s upper node's down to be (row_node)'s down
**	3) assign (row_node)'s lower node's up to be (row_node)'s up
**
**	Pretend the figures below are flipped 90 degrees
**	(i.e. vertical with uu at the top)
**
**	Before:
**	+------+  ->  +----------+      +------+
**	|  uu  |  <-  | row_node |	->  |  dd  |
**	+------+      +----------+  <-  +------+
**
**	After:	    /--------------\
**	+------+  --  +----------+  ->  +------+
**	|  uu  |  <-  | row_node |	->  |  dd  |
**	+------+  <-  +----------+  --  +------+
**		    \______________/
**
**	4) After hiding (row_node) within its column move to the next
**	node to the right and repeat 2) & 3) until row_node == col_node again.
*/

static void	cover_nodes(t_node *col_node)
{
	t_node	*row_node;
	t_node	*uu;
	t_node	*dd;

	row_node = col_node->right;
	while (row_node != col_node)
	{
		uu = row_node->up;
		dd = row_node->down;
		uu->down = dd;
		dd->up = uu;
		(row_node->col)->len--;
		row_node = row_node->right;
	}
}

/*
**	Function to "cover" a column (c) and related nodes
**	1) assign (c)'s previous column's next to be (c)'s next
**	2) assign (c)'s next column's prev to be (c)'s prev
**
**	Before:
**	+------+  ->  +-------+      +--------+
**	| prev |  <-  |   c   |	 ->  |  next  |
**	+------+      +-------+  <-  +--------+
**
**	After:	    /-----------\
**	+------+  --  +-------+  ->  +--------+
**	| prev |  <-  |   c   |	 ->  |  next  |
**	+------+  <-  +-------+  --  +--------+
**		    \___________/
**
**	Note: (c)'s 'next' and 'prev' pointers are still preserved!
**
**	Then, we proceed to go 'down' the column (c) and for each node in column:
**	1) cover all other nodes that are in the same row as our col_node
*/

void		cover_col(t_col *c)
{
	t_col	*l;
	t_col	*r;
	t_node	*col_node;

	l = c->prev;
	r = c->next;
	l->next = r;
	r->prev = l;
	col_node = c->head.down;
	while (col_node != &(c->head))
	{
		cover_nodes(col_node);
		col_node = col_node->down;
	}
}

/*
**	Functions to uncover 'covered' columns and nodes
**	We literally perform the reverse of the cover operations to uncover
**	our covered nodes and columns
*/

static void	uncover_nodes(t_node *col_node)
{
	t_node	*row_node;
	t_node	*uu;
	t_node	*dd;

	row_node = col_node->left;
	while (row_node != col_node)
	{
		uu = row_node->up;
		dd = row_node->down;
		uu->down = row_node;
		dd->up = row_node;
		(row_node->col)->len++;
		row_node = row_node->left;
	}
}

void		uncover_col(t_col *c)
{
	t_col	*l;
	t_col	*r;
	t_node	*col_node;

	col_node = c->head.up;
	while (col_node != &(c->head))
	{
		uncover_nodes(col_node);
		col_node = col_node->up;
	}
	l = c->prev;
	r = c->next;
	l->next = c;
	r->prev = c;
}
