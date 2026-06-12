/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Compare_Line_fd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 13:53:12 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/11 13:56:19 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line_bonus.h"

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"

int	main(int argc, char **argv)
{
	int			fd[100];
	int			finished[100];
	char		*line;
	int			active;
	int			i;
	const char	*colors[6];

	if (argc < 2)
		return (1);
	colors[0] = RED;
	colors[1] = GREEN;
	colors[2] = YELLOW;
	colors[3] = BLUE;
	colors[4] = MAGENTA;
	colors[5] = CYAN;
	i = 1;
	while (i < argc)
	{
		fd[i] = open(argv[i], O_RDONLY);
		if (fd[i] < 0)
			return (1);
		finished[i] = 0;
		i++;
	}
	active = argc - 1;
	while (active > 0)
	{
		i = 1;
		while (i < argc)
		{
			if (!finished[i])
			{
				line = get_next_line(fd[i]);
				if (!line)
				{
					finished[i] = 1;
					active--;
				}
				else
				{
					printf("%s[%s] %s%s",
						colors[(i - 1) % 6],
						argv[i],
						line,
						RESET);
					free(line);
				}
			}
			i++;
		}
	}
	i = 1;
	while (i < argc)
		close(fd[i++]);
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
13. Long_Text.txt
14. Test_Oscar.txt
15. Testers.txt
*/