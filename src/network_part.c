//
// Created by Oscar VERETOUT on 3/28/21.
//
#include "ft_ping.h"

void critical_check(bool val, const char *msg)
{
	if (val)
		return;
	puts(msg);
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

void	set_icmp_request(t_icmp_request_data *data)
{
	data->type = 8;
	data->code = 0;
	data->chcksum = 0;
	data->identifier = 0;
	data->seq_number = 0;
	ft_strncpy(data, data->data, 64);
	ft_strncpy("aswdertiopvbdfrtuiomnvfreuiopocvderdjkfkjufhgyui", (data->data) + 64, 48);
	data->chcksum = checksum(data->data, 64 + 48);
	ft_strncpy(data, data->data, 64);
}

void	set_sock_addr_in(struct sockaddr_in *a_in)
{
	a_in->sin_family = AF_INET;
	a_in->sin_port = htons(3490);
}

//TODO: change the size if need be for ipv6. l17
//TODO: if the protocol is ipv6, change the Type of the message. l19
// accordingly(https://en.wikipedia.org/wiki/Ping_(networking_utility)#ICMP_packet)
//TODO: Change AF_INET to AFINET6 when using ipv6. l29

void establish_connection(t_master *m)
{
	t_networking n;
	n.ping_loop = true;

	n.sd = socket(m->domain, SOCK_DGRAM, 0);
	critical_check(n.sd != -1, "Unable to create a socket.");
	critical_check(inet_pton(AF_INET, "127.0.0.1", &(n.a_in.sin_addr)) == 1, "Failed to convert localhost to binary address.");
	set_icmp_request(&(n.data));
	sendto(n.sd, &(n.data), PACKET_SIZE, 0, &(n.a_in), sizeof(struct in_addr));
	close(n.sd);
}