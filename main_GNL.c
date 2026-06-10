/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_GNL.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:56:52 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/10 16:32:43 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("12. HW42.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("[%s]", line);
		free(line);
		line = get_next_line(fd);
	}
    printf("\n");
	close(fd);
	return (0);
}
/*List of files:
01. A.txt
02. About_get_next_line.txt
03. An.txt
04. AnBn.txt
05. Coding_get_next_line.txt
06. Empty.txt
07. Everything_Emptiness_Poem.txt
08. n.txt
09. nnn.txt
10. Random.txt
11. Videos_get_next_line.txt
12. HW42.txt
*/