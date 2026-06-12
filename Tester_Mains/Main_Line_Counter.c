/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Line_Counter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:14:12 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/10 18:32:29 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		count;

	if (argc != 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	count = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		printf("Line %d: [%s]", ++count, line);
		free(line);
	}
	printf("\nTotal lines: %d\n", count);
	close(fd);
	return (0);
}
/*For example:
./a.out "07. Everything_Emptiness_Poem.txt"
would give me the line numbers (25)
and content of each line in the file*/
