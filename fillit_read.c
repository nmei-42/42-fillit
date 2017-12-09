/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 14:44:49 by nmei              #+#    #+#             */
/*   Updated: 2017/12/07 19:27:26 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fillit.h"

#define IS_VALIDCHAR(x) ((x) == '.' || (x) == '#')
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/*
**	count_neighbors()
**	check if left, up, right, down positions from row, col contain a '#'
**
**	Because count_neighbors() requires a '#' to be found we can make it do
**	triple duty by 2) incrementing our piece->block_count variable and
**	3) determining the min and max row and col coordinates for all '#'s
**
**	left:  row, col - 1
**	up:	   row - 1, col
**	right: row, col + 1
**	down:  row + 1, col
**
**	Calls itself recursively so that it can check if boundry conditions
**	are enforced before trying to index values of the board.
**
**	If the block count is 4 or greater then our putative piece has 5
**	or more blocks so we shouldn't waste time counting neighbors...
*/

static void		count_neighbors(t_tetlst *piece, int row, int col, int level)
{
	if (level == 0)
	{
		if (piece->block_count < 4)
		{
			piece->row_coords[piece->block_count] = row;
			piece->col_coords[piece->block_count] = col;
			piece->min_row = MIN(piece->min_row, row);
			piece->min_col = MIN(piece->min_col, col);
			piece->max_row = MAX(piece->max_row, row);
			piece->max_col = MAX(piece->max_col, col);
			piece->block_count++;
			count_neighbors(piece, row, col - 1, 1);
			count_neighbors(piece, row - 1, col, 1);
			count_neighbors(piece, row, col + 1, 1);
			count_neighbors(piece, row + 1, col, 1);
		}
		else
			piece->is_valid = 0;
	}
	else if ((row >= 0 && row <= 3) && (col >= 0 && col <= 3))
	{
		if (piece->board[row][col] == '#')
			piece->neighbors++;
	}
}

/*
**	check_tetra()
**	Even if a tetra board is read in successfully, doesn't necessarily
**	indicate that the board contains a valid tetrinimo
**
**	To check if this is the case, we count the number of '#'s in a board
**	Additionally, when we encounter a '#' we check it's 4 neighbors to see
**	if they are also '#'s.
**
**	A valid piece will always have 4 blocks and will return a neighbor count
**	of either 6 (all pieces except square) or 8 (the square). Any other counts
**	means that the piece is not a valid & connected tetrinimo!!!
*/

static void		check_tetra(t_tetlst *piece)
{
	int row;
	int col;

	if (piece->is_valid)
	{
		row = -1;
		while (++row < 4)
		{
			col = -1;
			while (++col < 4)
			{
				if (piece->board[row][col] == '#')
					count_neighbors(piece, row, col, 0);
			}
		}
	}
	if (piece->block_count != 4 || piece->name[0] > 'Z')
		piece->is_valid = 0;
	if (!(piece->neighbors == 6 || piece->neighbors == 8))
		piece->is_valid = 0;
}

/*
**	read_tetra()
**	Function takes in a file descriptor and t_tetlst struct.
**	The ssize_t ret and char *buf are wonky hacks to get under the 25 line
**	limit so pretend it's equivalent to defining ret and buf (see read_file()).
**
**	The function will read 4 lines from the given fd and attempt to
**	read them into the t_tetlst->board variable.
**
**	Formatting failures will result in the t_tetlst->is_valid flag being
**	set to 0.
**
**	In the edge case that fd describes a blank file, read_tetra() will
**	return the ret value (which will be 0) so that the t_tetlst->is_valid
**	flag can be set to 0 in the read_file() function.
*/

static ssize_t	read_tetra(int fd, t_tetlst *piece, ssize_t ret, char *buf)
{
	size_t	col;
	size_t	row;

	row = 0;
	while (row < 4 && piece->is_valid && (ret = read(fd, buf, 5)))
	{
		if (buf[ret - 1] == '\n' && ret == 5)
		{
			buf[ret - 1] = '\0';
			col = 0;
			while (buf[col])
			{
				if (IS_VALIDCHAR(buf[col]))
					(piece->board)[row][col] = buf[col];
				else
					piece->is_valid = 0;
				col++;
			}
			(piece->board)[row][col] = '\0';
		}
		else
			piece->is_valid = 0;
		row++;
	}
	return (ret);
}

/*
**	read_file - Function to read our fillit file. We read 4 lines at a time
**	with the read_tetra function and then we proceed to read 1 line to check
**	that it is \n character.
**
**	The function takes in a filepath, and tetrinimo list ptr ptr (which stores
**	info about every read in tetrinimo) and a 'name' parameter which gives
**	each tetra piece a 1 character 'name'.
*/

void			read_file(const char *fpath, t_tetlst **start, char name)
{
	int			fd;
	char		buf[5];
	ssize_t		ret;

	ret = 0;
	if ((fd = open(fpath, O_RDONLY)) != -1)
	{
		tet_lstadd(start, tet_lstnew(name++));
		if (read_tetra(fd, *start, ret, buf) == 0)
			(*start)->is_valid = 0;
		check_tetra(*start);
		while ((*start)->is_valid && (ret = read(fd, buf, 1)) == 1)
		{
			if (buf[0] == '\n')
			{
				tet_lstadd(start, tet_lstnew(name++));
				if (read_tetra(fd, *start, ret, buf) == 0)
					(*start)->is_valid = 0;
				check_tetra(*start);
			}
			else
				(*start)->is_valid = 0;
		}
		close(fd);
	}
}
