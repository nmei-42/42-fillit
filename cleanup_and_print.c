/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/08 09:57:30 by nmei              #+#    #+#             */
/*   Updated: 2017/12/08 18:18:52 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "fillit.h"

int		decode(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'Z')
		return (c + 10 - 'A');
	else if (c >= 'a' && c <= 'z')
		return (c + 10 - 'a');
	return (0);
}

void	emancipate_xcover(t_xcover *xcov)
{
	free(xcov->col_names);
	free(xcov->col_arr);
	xcov->root = NULL;
	nn_lstfree(&(xcov->nnlst));
	free(xcov->node_arr);
	free(xcov->choice);
}

void	fmt_result(char *result, int bsize)
{
	int	row;
	int	col;

	row = -1;
	while (++row < bsize)
	{
		col = -1;
		while (++col < bsize + 1)
		{
			if (col != bsize)
				*result++ = '.';
			else
				*result++ = '\n';
		}
	}
	*result = '\0';
}

void	update_result(char *result, t_node *choice, int bsize)
{
	char	*n;
	char	*c1;
	char	*c2;
	char	*c3;
	char	*c4;

	n = choice->col->name;
	c1 = choice->right->col->name;
	c2 = choice->right->right->col->name;
	c3 = choice->right->right->right->col->name;
	c4 = choice->right->right->right->right->col->name;
	result[decode(c1[0]) * (bsize + 1) + decode(c1[1])] = *n;
	result[decode(c2[0]) * (bsize + 1) + decode(c2[1])] = *n;
	result[decode(c3[0]) * (bsize + 1) + decode(c3[1])] = *n;
	result[decode(c4[0]) * (bsize + 1) + decode(c4[1])] = *n;
}

/*
**	disp_res_n_clean
**
**	1) Generate an appropriately sized string to fit a board with edge length
**	   'bsize'.
**	2) Fill the string with '.'s and '\n's
**	3) Read out the xcov->choice variable which stores the nodes (row starts)
**	   that the DLX algorithm chose when trying to find an exact cover. We
**	   unpack each choice and update our result string appropriately.
**	4) Print the result.
**	5) Clean up all the malloc'd data structures we allocated.
*/

void	disp_res_n_clean(t_xcover *xcov, t_tetlst **tetra_pieces, int bsize)
{
	size_t	tlst_len;
	size_t	i;
	t_node	**choice_arr;
	char	*result;

	result = (char *)malloc((1 + bsize * (bsize + 1)) * sizeof(*result));
	fmt_result(result, bsize);
	tlst_len = tet_lstlen(*tetra_pieces);
	choice_arr = xcov->choice;
	i = 0;
	while (i < tlst_len)
		update_result(result, choice_arr[i++], bsize);
	write(1, result, (bsize * (bsize + 1)));
	emancipate_xcover(xcov);
	tet_lstfree(tetra_pieces);
	free(result);
}
