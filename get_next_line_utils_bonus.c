/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omatalla <omatalla@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:15:15 by omatalla          #+#    #+#             */
/*   Updated: 2026/06/09 18:30:18 by omatalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(const char *s)
{
	size_t		i;
	size_t		length;
	char		*target;

	length = 0;
	while (s[length] != '\0')
		length++;
	target = (char *) malloc(sizeof(char) * (length + 1));
	if (target == NULL)
		return (NULL);
	i = 0;
	while (i < length)
	{
		target[i] = s[i];
		i++;
	}
	target[i] = '\0';
	return (target);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)&s[i]);
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '\0')
		return (i);
	while (s[i] != '\0')
		i++;
	return (i);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*pdest;
	unsigned char	*psrc;

	if (dest == NULL && src == NULL)
		return (NULL);
	pdest = (unsigned char *)dest;
	psrc = (unsigned char *)src;
	while (n--)
		*pdest++ = *psrc++;
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*concat_new_str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	concat_new_str = (char *)malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (concat_new_str == NULL)
		return (NULL);
	ft_memcpy(concat_new_str, s1, len_s1);
	ft_memcpy(concat_new_str + len_s1, s2, len_s2);
	concat_new_str[len_s1 + len_s2] = '\0';
	return (concat_new_str);
}
