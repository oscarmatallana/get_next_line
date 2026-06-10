/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 12:40:07 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/10 13:50:58 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*append_buffer(char *line_container, char *buffer)
{
	char	*tmp_line_container;

	tmp_line_container = ft_strjoin(line_container, buffer);
	free(line_container);
	return (tmp_line_container);
}

/* In a nutshell:
- append_buffer() is responsible for 
updating the container after a read. 
- It joins the newly read bytes 
to the existing unread data 
and frees the old container.
- This is to make sure
I don´t leak the old line_container*/

char	*read_and_store(int fd, char *line_container)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	if (!line_container)
		line_container = ft_strdup("");
	if (!line_container)
		return (free(buffer), NULL);
	while (bytes_read > 0 && !ft_strchr(line_container, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(buffer), free(line_container), NULL);
		buffer[bytes_read] = '\0';
		line_container = append_buffer(line_container, buffer);
		if (!line_container)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (line_container);
}

/* - Guarantees that line_container is a valid string
and reads data until a newline or EOF is reached.
- I didn't want ft_strchr(NULL, '\n')
- line_container = ft_strdup("")
means malloc'd memory containing "".
If read fails, 
I free both buffer and line_container 
to avoid leaks.
bytes_read = 1;
allows the loop to start.
bytes_read contains a meaningful value
after the first read.
- My algorithm in words:
allocate buffer
initialize container if needed
while (...)
{
	read

	check errors

	null terminate buffer

	append_buffer()

	check failure
}
free buffer
return container
- In a nutshell:
1. Allocate buffer.
2. Initialize line_container if needed.
3. Read until newline or EOF.
4. Append newly read data.
5. Handle errors and allocation failures.
6. Free buffer.
7. Return accumulated data.*/

char	*extract_line(char *line_container)
{
	char	*line;
	size_t	line_length;

	if (!line_container || line_container[0] == '\0')
		return (NULL);
	line_length = 0;
	while (line_container[line_length] != '\n'
		&& line_container[line_length] != '\0')
		line_length++;
	if (line_container[line_length] == '\n')
		line_length++;
	line = malloc((line_length + 1) * sizeof(char));
	if (!line)
		return (NULL);
	ft_memcpy(line, line_container, line_length);
	line[line_length] = '\0';
	return (line);
}

/* - In an nutshell:
1. Determine the final length of the line,
including the newline if present.
2. Allocate exactly that many characters + '\0'.
3. Copy exactly that many characters.
4. Add '\0'.*/

char	*update_line_container(char *line_container)
{
	char	*updated_container;
	size_t	i;

	i = 0;
	while (line_container[i] != '\n'
		&& line_container[i] != '\0')
		i++;
	if (line_container[i] == '\n')
		i++;
	if (line_container[i] == '\0')
		return (free(line_container), NULL);
	updated_container = ft_strdup(&line_container[i]);
	free(line_container);
	return (updated_container);
}

/* -I replaced a whole line of code
with ft_strdup(&line_container[i]):
j = 0;
	while (line_container[i] != '\0')
		updated_container[j++] = line_container[i++];
	updated_container[j] = '\0';
	free(line_container);
	return (updated_container);*/
/*- In a nutshell:
1. Find the index of the first newline or end of string.
2. If newline is found, move past it.
3. If end of string is reached, free container and return NULL.
4. Duplicate the remaining string starting from the new index.
5. Free the old container.
6. Return the new container with the remaining unread data.*/

char	*get_next_line(int fd)
{
	static char	*line_container;
	char		*extracted_line;

	if (fd < 0 || BUFFER_SIZE <= 0
		|| read(fd, 0, 0) < 0)
	{
		free(line_container);
		line_container = NULL;
		return (NULL);
	}
	line_container = read_and_store(fd, line_container);
	if (!line_container)
		return (NULL);
	extracted_line = extract_line(line_container);
	if (!extracted_line)
	{
		free(line_container);
		line_container = NULL;
		return (NULL);
	}
	line_container = update_line_container(line_container);
	return (extracted_line);
}
/*- line_container stores all unread data between calls.
- extract_line() creates the line to return.
- update_line_container() removes the returned line
and keeps only the remaining unread characters.*/