# fillit
An implementation of [Knuth's Dancing Links](https://en.wikipedia.org/wiki/Dancing_Links) to determine the smallest square that will fit 1-26 tetromino (a.k.a. Tetris) pieces

### Installation
To generate an executable for this project run `make` in the root of the project directory.

### Usage
After running `make` to generate the fillit executable you can run the program with `./fillit [valid_fillit_file]`

A valid fillit file will contain between 1-26 (includsive) tetrominos. Each tetromino will be described in 4 lines (followed by a newline) with '#' representing a block in the tetromino and '.' representing an empty space. Each block of a tetromino must touch at least one other block in one of the 4 cardinal directions (up, down, left, right).

example:

```
$> cat -e valid_fillit_file
....$
##..$
.#..$
.#..$
$
....$
####$
....$
....$
$
#...$
###.$
....$
....$
$
....$
##..$
.##.$
....$
$>
```

### Output
The program will find and print out the smallest *square* that will fit all the pieces described in the valid_fillit_file and will prioritize placing earlier encountered pieces in the top-left corner of the square if possible.

example:

```
$> ./fillit valid_fillit_file | cat -e
DDAA$
CDDA$
CCCA$
BBBB$
$>
```
### Readings and resources that were instrumental for implementing this project

[Knuth's Dancing Links paper](https://arxiv.org/abs/cs/0011047)

[Knuth's Algorithm X](https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X)
