//
// Created by Oscar VERETOUT on 3/28/21.
//

#include "ft_ping.h"


bool ft_strcmp(char *s1, char *s2)
{
	int i = 0;

	while (s1[i])
	{
		if (s1[i] != s2[i])
			return false;
		i++;
	}
	return true;
}

void	ft_strcpy(const char *src, char *dest)
{
	unsigned int i = 0;

	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
}

void	ft_strncpy(const char *src, char *dest, int len)
{
	int i = 0;

	while (i != len)
	{
		dest[i] = src[i];
		i++;
	}
}