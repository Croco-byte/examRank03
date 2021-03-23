#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct	s_zone
{
	int	width;
	int	height;
	char	background;
}		t_zone;

typedef struct	s_list
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	drawchar;
}		t_list;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	fail(char *error)
{
	write(1, error, ft_strlen(error));
	return (1);
}

int	clean(FILE *file, char *draw)
{
	fclose(file);
	if (draw)
		free(draw);
	return (1);
}

int	valid_zone(t_zone *zone)
{
	if (zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300)
		return (1);
	return (0);
}

char	*get_zone(FILE *file, t_zone *zone)
{
	int	i;
	int	count;
	char	*result;

	i = 0;
	if ((count = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (!(valid_zone(zone)))
		return (0);
	if (!(result = malloc((zone->width * zone->height) * sizeof(char))))
		return (0);
	while (i < zone->width * zone->height)
	{
		result[i] = zone->background;
		i++;
	}
	return (result);
}

int	valid_tmp(t_list *tmp)
{
	if (tmp->width > 0.00000000 && tmp->height > 0.00000000 && (tmp->type == 'r' || tmp->type == 'R'))
		return (1);
	return (0);
}

int	is_rec(int x, int y, t_list *tmp)
{
	float one = 1.00000000;

	if (x < tmp->x || x > tmp->x + tmp->width || y < tmp->y || y > tmp->y + tmp->height)
		return (0);
	if (x - tmp->x < one || (tmp->x + tmp->width) - x < one || y - tmp->y < one || (tmp->y + tmp->height) - y < one)
		return (2);
	return (1);
}

void	draw_rec(char **draw, t_zone *zone, t_list *tmp)
{
	int	x;
	int	y;
	int	rec;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			rec = is_rec(x, y, tmp);
			if ((tmp->type == 'r' && rec == 2) || (tmp->type == 'R' && rec != 0))
				(*draw)[(y * zone->width) + x] = tmp->drawchar;
			x++;
		}
		y++;
	}
}

int	drawing(FILE *file, char **draw, t_zone *zone)
{
	int	count;
	t_list	tmp;

	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.drawchar)) == 6)
	{
		if (!(valid_tmp(&tmp)))
			return (0);
		draw_rec(draw, zone, &tmp);
	}
	if (count != -1)
		return (0);
	return (1);
}

void	print_draw(char *draw, t_zone *zone)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		write(1, &draw[i * zone->width], zone->width);
		write(1, "\n", 1);
		i++;
	}
}



int	main(int argc, char **argv)
{
	FILE	*file;
	char	*draw;
	t_zone	zone;

	if (argc != 2)
		return (fail("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (fail("Error: Operation file corrupted\n"));
	if (!(draw = get_zone(file, &zone)))
		return (clean(file, 0) && fail("Error: Operation file corrupted\n"));
	if (!(drawing(file, &draw, &zone)))
		return (clean(file, draw) && fail("Error: Operation file corrupted\n"));
	print_draw(draw, &zone);
	clean(file, draw);
	return (0);
}
