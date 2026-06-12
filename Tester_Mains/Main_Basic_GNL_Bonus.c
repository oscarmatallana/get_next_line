/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Basic_GNL_Bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 13:20:04 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/11 13:32:53 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line_bonus.h"

#define RESET   "\033[0m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define BR_RED     "\033[91m"
#define BR_GREEN   "\033[92m"
#define BR_YELLOW  "\033[93m"
#define BR_BLUE    "\033[94m"
#define BR_MAGENTA "\033[95m"
#define BR_CYAN    "\033[96m"
#define GRAY       "\033[90m"
#define ORANGE     "\033[38;5;208m"

int	main(void)
{
	int			fd[15];
	char		*line;
	int			finished[15];
	int			active_files;
	int			i;

	const char	*files[15] = {
		"01. A.txt",
		"02. About_get_next_line.txt",
		"03. An.txt",
		"04. AnBn.txt",
		"05. Coding_get_next_line.txt",
		"06. Empty.txt",
		"07. Everything_Emptiness_Poem.txt",
		"08. n.txt",
		"09. nnn.txt",
		"10. Random.txt",
		"11. Videos_get_next_line.txt",
		"12. HW42.txt",
		"13. Long_Text.txt",
		"14. Test_Oscar.txt",
		"15. Testers.txt"
	};

	const char	*colors[15] = {
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		BR_RED,
		BR_GREEN,
		BR_YELLOW,
		BR_BLUE,
		BR_MAGENTA,
		BR_CYAN,
		GRAY,
		ORANGE
	};

	i = 0;
	while (i < 15)
	{
		fd[i] = open(files[i], O_RDONLY);
		finished[i] = 0;
		if (fd[i] < 0)
			return (1);
		i++;
	}
	active_files = 15;
	while (active_files > 0)
	{
		i = 0;
		while (i < 15)
		{
			if (!finished[i])
			{
				line = get_next_line(fd[i]);
				if (!line)
				{
					finished[i] = 1;
					active_files--;
				}
				else
				{
					printf("%sFD %d (%s): %s%s",
						colors[i],
						fd[i],
						files[i],
						line,
						RESET);
					free(line);
				}
			}
			i++;
		}
	}
	i = 0;
	while (i < 15)
	{
		close(fd[i]);
		i++;
	}
	return (0);
}

/*The bonus requirement is: 
Handle multiple file descriptors simultaneously
Read one line from A.
Read one line from B.
Read one line from C.
Go back to A.
Go back to B.
Go back to C.
In a nutshel, this main does:
Step 1: Create the list of files
Step 2: Create an array of file descriptors
Step 3: Open all files
Step 4: Track which files are finished
Step 5: Count active files
Step 6: Main test loop:
while (active_files > 0)
continue while at least one file
still has unread lines.
Step 7: Visit every file
Step 8: Skip finished files:
if (!finished[i]) or if (finished[i] == 0)
Only read from files that
still contain unread lines.
Step 9: Read one line:
line = get_next_line(fd[i]);
The bonus succeeds only if
the function remembers
where it left off.
Step 10: Detect EOF:
if (!line) or if (line == NULL)
then no more lines available,
Mark this file as completed
and reduce the count of active files.
Step 11: Print the line*/