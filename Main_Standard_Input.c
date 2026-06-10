/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   standard_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:51:34 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/10 17:58:08 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	char	*line;

	printf("Write input:\n");
	line = get_next_line(0);
	printf("Read: [%s]", line);
	free(line);
	return (0);
}
