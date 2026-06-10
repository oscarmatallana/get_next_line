/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_GNL.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:56:52 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/10 16:09:44 by omatalla         ###   ########.fr       */
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

	fd = open("About_get_next_line.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("[%s]", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
/*List of files:
1. A.txt
2. About_get_next_line.txt
3. An.txt
4. AnBn.txt
5. Coding_get_next_line.txt
6. Empty.txt
7. Everything_Emptiness_Poem.txt
8. n.txt
9. nnn.txt
10. Random.txt
11. Videos_get_next_line.txt
*/