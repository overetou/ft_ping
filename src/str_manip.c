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
