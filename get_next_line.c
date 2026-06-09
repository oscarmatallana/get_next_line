/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 12:40:07 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/09 18:15:25 by omatalla         ###   ########.fr       */
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

/*append_buffer() is responsible for 
updating the container after a read. 
It joins the newly read bytes 
to the existing unread data 
and frees the old container.
This is to make sure
I don´t leak the old line_container*/

char	*free_all(char *buffer, char *line_container)
{
	free(buffer);
	free(line_container);
	return (NULL);
}

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
			return (free_all(buffer, line_container));
		buffer[bytes_read] = '\0';
		line_container = append_buffer(line_container, buffer);
		if (!line_container)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (line_container);
}

/*Guarantees that line_container is a valid string and
reads data until a newline or EOF is reached.
I didn't want ft_strchr(NULL, '\n')
My algorithm in words:
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
return container*/

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
/*line_container stores all unread data between calls.
extract_line() creates the line to return.
update_line_container() removes the returned line
and keeps only the remaining unread characters.*/