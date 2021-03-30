//
// Created by Oscar VERETOUT on 3/28/21.
//
#include "ft_ping.h"

#include <string.h>//TODO: Remove this line
#include <errno.h>//TODO: Remove this line

void critical_check(bool val, const char *msg)
{
	if (val)
		return;
	fprintf(stderr, "System report: \"%s\".\n", strerror(errno));
	fputs(msg, stderr);
	exit(0);
}

unsigned short checksum(void *b, int len)
{
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;

	while (len > 1)
	{
		sum += *buf;
		buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

uint16_t	ip_checksum(void *vdata, size_t len)
{
	char	*casted = (char*)vdata;
	uint32_t	accumulator = 0xffff;
	uint16_t	word;

	for (size_t i = 0; i + 1 < len; i += 2)
	{
		ft_strncpy(casted + i, &word, 2);
		accumulator += ntohs(word);
		if (accumulator > 0xffff)
			accumulator -= 0xffff;
	}
	if (len & 1)
	{
		word = 0;
		ft_strncpy(casted + len - 1, &word, 1);
		accumulator += ntohs(word);
		if (accumulator > 0xffff)
			accumulator -= 0xffff;
	}
	return htons(~accumulator);
}

void	set_icmp_request(t_icmp_request_data *data)
{
	data->type = 8;
	data->code = 0;
	data->chcksum = 0;
	data->identifier = 0;
	data->seq_number = 0;
	ft_strncpy((char*)data, data->data, 64);
	ft_strncpy("aswdertiopvbdfrtuiomnvfreuiopocvderdjkfkjufhgyui", (data->data) + 64, 48);
	data->chcksum = checksum(data->data, 64 + 48);
	ft_strncpy((char*)data, data->data, 64);
}

void	set_sock_addr_in(struct sockaddr_in *a_in)
{
	a_in->sin_family = AF_INET;
}

//TODO: change the size if need be for ipv6. l17
//TODO: if the protocol is ipv6, change the Type of the message. l19
// accordingly(https://en.wikipedia.org/wiki/Ping_(networking_utility)#ICMP_packet)
//TODO: Change AF_INET to AFINET6 when using ipv6. l29

void establish_connection(t_master *m)
{
	t_networking n;
	n.ping_loop = true;
	int				hdrincl = 0;

	n.sd = socket(m->domain, SOCK_RAW, IPPROTO_ICMP);
	critical_check(
		n.sd != -1,
		"Unable to create a socket.");
	
	critical_check(
		setsockopt(n.sd, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)),
		"setsockopt failure.");

	const size_t	req_size = 8;
	struct icmphdr	req;
	req.type = 8;
	req.code = 0;
	req.checksum = 0;
	req.un.echo.id = htons(rand());
	req.un.echo.sequence = htons(1);
	req.checksum = ip_checksum(&req, req_size);

	//critical_check(inet_pton(AF_INET, "127.0.0.1", &(n.a_in.sin_addr)) == 1, "Failed to convert localhost to binary address.");

	critical_check(
		sendto(n.sd, &req, req_size, 0, res->ai_addr, res->ai_addrlen) != -1,
		"sendto() failed.");

	close(n.sd);
}