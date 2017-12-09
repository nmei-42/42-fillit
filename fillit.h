/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/02 14:26:06 by nmei              #+#    #+#             */
/*   Updated: 2017/12/08 18:06:39 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
**	42 Fillit implementation using Donald Knuth's DLX algorithm
**	https://arxiv.org/pdf/cs/0011047.pdf
*/

#ifndef FILLIT_H
# define FILLIT_H
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

/*
**	t_node - node struct as described by Knuth. Nodes represent possible
**	combinations that fulfil a subset of constraints (see below).
*/
typedef struct		s_node
{
	struct s_node	*left;
	struct s_node	*right;
	struct s_node	*up;
	struct s_node	*down;
	struct s_col	*col;
}					t_node;

/*
**	t_col - Column struct as described by Knuth. Columns describe
**	constraints in an exact cover problem.
*/
typedef struct		s_col
{
	t_node			head;
	size_t			len;
	char			name[8];
	struct s_col	*prev;
	struct s_col	*next;
}					t_col;

/*
**	node namelist
**	The namelist->row will always be 13 + 1 characters long
**	piece name (1 chars)
**	coord names (3 chars) * 4
**	null term (1 char)
*/
typedef struct		s_nnlst
{
	char			row[14];
	struct s_nnlst	*next;
}					t_nnlst;

/*
**	The eXact cover struct contains critical variables for performing
**	dancing links. As a bonus, most malloc'd objects in this fillit
**	project happen to also be contained in this structure making
**	re-initialization of the eXact cover struct much much easier...
*/
typedef struct		s_xcover
{
	char			*col_names;
	t_col			*col_arr;
	t_col			*root;
	t_nnlst			*nnlst;
	t_node			*node_arr;
	t_node			**choice;
}					t_xcover;

/*
**	t_tetlst - Each element in the tetrinimo list structure describes various
**	aspects of 1 tetrinimo piece that has been read in from the file.
*/
typedef struct		s_tetlst
{
	char			board[4][5];
	int				is_valid;
	struct s_tetlst	*next;
	int				block_count;
	int				neighbors;
	int				row_coords[4];
	int				col_coords[4];
	int				min_row;
	int				min_col;
	int				max_row;
	int				max_col;
	char			name[1];
}					t_tetlst;

/*
**	main.c - includes various misc. functions that are useful
*/
int					mod_strcmp(char *s1, char *s2);
void				mod_strcpy(char *dest, char *src);

/*
**	tetra_list_utils.c - Tetrinimo linked list utilities.
*/
t_tetlst			*tet_lstnew(char name);
void				tet_lstadd(t_tetlst **alst, t_tetlst *new);
size_t				tet_lstlen(t_tetlst *begin_list);
void				tet_lstrev(t_tetlst **begin_list);
void				tet_lstfree(t_tetlst **begin_list);

/*
**	node_namelist_utils.c - node name linked list utilities.
*/
t_nnlst				*nn_lstnew(t_tetlst *piece, int row, int col);
void				nn_lstadd(t_nnlst **alst, t_nnlst *new);
size_t				nn_lstlen(t_nnlst *begin_list);
void				nn_lstrev(t_nnlst **begin_list);
void				nn_lstfree(t_nnlst **begin_list);

/*
**	fillit_read.c - Functions to read fillit files and load them into
**	tetrinimo list structs. Also contains functions to check that a read
**	in tetrinimo board contains a valid tetrinimo.
*/
void				read_file(const char *fpath, t_tetlst **start, char name);

/*
**	create_xcover_cols.c - Functions to create the columns (a.k.a.
**	restrictions) in our eXact cover problem.
*/
char				*make_col_names(int slen, t_tetlst *pieces,
									ssize_t tlst_len);
void				link_columns(t_xcover *xcov);

/*
**	create_xcover_rows.c - Functions to create the row nodes (a.k.a.
**	possible piece positions) in our eXact cover problem.
*/
void				make_node_namelst(t_nnlst **nnlst, t_tetlst *pieces,
										int bsize);
void				link_rows(t_xcover *xcov);

/*
**	dancing_links_ops.c - Dancing links operations
**	NOTE: nodes operations MUST be defined before the col cover & uncover ops.
*/
void				cover_col(t_col *c);
void				uncover_col(t_col *c);

/*
**	fillit_dlx_solve.c - This is the part that solves the fillit...
*/
void				prep_xcover(t_xcover *xcov, t_tetlst **tetra_pieces,
								int bsize);
int					dlx_solve(t_xcover *xcov, int level);
void				cover_row_cols(t_node *curr_node);
void				uncover_row_cols(t_node *curr_node);

/*
**	cleanup_and_print.c - functions to print the final result as well as to
**	free all malloc'd data structures.
*/
void				emancipate_xcover(t_xcover *xcov);
void				disp_res_n_clean(t_xcover *xcov, t_tetlst **tetra_pieces,
									int bsize);

#endif
