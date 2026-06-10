/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_GNL.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:56:52 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/10 16:05:34 by omatalla         ###   ########.fr       */
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
A.txt
About_get_next_line.txt
An.txt
AnBn.txt
Coding_get_next_line.txt
Empty.txt
Everything_Emptiness_Poem.txt
n.txt
nnn.txt
Random.txt
Videos_get_next_line.txt
*/