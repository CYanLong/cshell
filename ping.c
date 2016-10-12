#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>


#define ICMP_SIZE	(sizeof(struct icmp))

#define ICMP_ECHO 	   	8
#define ICMP_ECHOREPLY 	0
#define BUF_SIZE 			1024
#define NUM				10

#define  UCHAR unsigned char
#define USHORT unsigned short
#define UINT unsigned int

struct icmp{
	UCHAR	type;
	UCHAR 	code;
	USHORT	checksum;
	USHORT	id;
	USHORT	sequence;
	struct timeval timestamp; 
};

struct ip{
	#if __BYTE_ORDER == __LITTLE_ENDIAN
	UCHAR	hlen:4;
	UCHAR	version:4;
	#endif
	#if __BYTE_ORDER == __BIG_ENDIAN
	UCHAR	version:4;
	UCHAR	hlen:4;
	#endif
	UCHAR	tos;
	USHORT	len;
	USHORT	id;
	USHORT	offset;
	UCHAR	ttl;
	UCHAR	protocol;
	USHORT	checksum;
	struct in_addr	ipsrc;
	struct in_addr	ipdest;
};

char buf[BUF_SIZE] = {0};

USHORT checkSUm(USHORT *, int);
float timediff(struct timeval *, struct timeval *);
void pack(struct icmp *, int);
int unpack(char *, int, char *);

int main(int argc, char** argv){

	struct hostent	*host;
	struct sockaddr_in	to;
	struct sockaddr_in	from;
	struct icmp sendicmp;

	int sockfd;
	int nsend = 0;
	int nreceived = 0;
	struct in_addr inaddr;
	int n;
	memset(&from, 0, sizeof(struct sockaddr_in));
	memset(&to, 0, sizeof(struct sockaddr_in));

	if(argc < 2){
		printf("Use: %s <hostname>\n", argv[0]);
		exit(1);
	}
	
	to.sin_family = AF_INET;

	if((host = gethostbyname(argv[1])) == NULL){
		if(inet_aton(argv[1], &inaddr) == 0){
			fprintf(stderr, "Invalid address\n");
			exit(-1);
		}
		to.sin_addr = inaddr;
	}else{
		to.sin_addr = *(struct in_addr *)host->h_addr_list[0];
	}

	
	if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1){
		printf("socket error\n");
		return -1;
	}
	
	int num;
	for(num = 0; num < NUM; num++){

		memset(&sendicmp, 0, ICMP_SIZE);
		pack(&sendicmp, num + 1);

		if(sendto(sockfd, &sendicmp, ICMP_SIZE, 
			0, (struct sockaddr*)&to, sizeof(to)) == -1){
			perror("send to error\n");
			continue;
		}
		nsend++;//send success
		
		int fromsize = sizeof(struct sockaddr);
		if((n = recvfrom(sockfd, buf, BUF_SIZE, 
			0, (struct sockaddr *)&from, &fromsize))< 0){
			perror("recvfrom error\n");
			continue;
		}
		
		if(unpack(buf, n, inet_ntoa(from.sin_addr)) == -1){
			printf("unpack() error \n");
			continue;
		}
		nreceived++; //recv success
		sleep(1);
	}
	
	printf("--- %s ping statistics ---\n", argv[1]);
	printf("%d packets transmitted, %d recevied, %%%d packet loss, time: %fms\n", 
		nsend, nreceived, (nsend - nreceived)/nsend * 100);
	return 0;
}

unsigned short checkSum(unsigned short *addr, int len){
	unsigned int sum = 0;
	while(len > 1){
		sum += *addr++;
		len -= 2;
	}

	if(len == 1){
		sum += *(unsigned char *)addr;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	return (unsigned short) ~sum;
}

float timediff(struct timeval *begin, struct timeval *end){
	int n;
	n = (end->tv_sec - begin->tv_sec) * 1000000
		+ (end->tv_usec - begin->tv_usec);

	return (float) (n / 1000);
}
void pack(struct icmp *icmp, int sequence){
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->id = getpid();
	icmp->sequence = sequence;
	gettimeofday(&icmp->timestamp, 0);
	icmp->checksum = checkSum((USHORT *)icmp, ICMP_SIZE);
}

int unpack(char * buf, int len, char * addr){
	int i, ipheadlen;
	struct ip *ip;
	struct icmp * icmp;
	float rtt;
	struct timeval end;

	ip = (struct ip *)buf;
	//printf("ip->hlen: %d\n", ip->hlen);
	ipheadlen = ip->hlen << 2;
	
	icmp = (struct icmp *)(buf + ipheadlen);

	len -= ipheadlen; //icmp length

	if(len < 8){
		perror("recv icmp packet format error\n");
	}

	if(icmp->type != ICMP_ECHOREPLY || 
		icmp->id != getpid()){
		return -1;
	}

	gettimeofday(&end, 0);
	rtt = timediff(&icmp->timestamp, &end);

	printf("%d bytes from %s : icmp_seq=%u, ttl=%d, time=%fms \n",
		len, addr, icmp->sequence, ip->ttl, rtt);
	return 0;
}