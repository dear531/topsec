#include <asm/types.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>



#if 1
	struct {
		struct nlmsghdr nh;
		struct ifinfomsg if1;
		char attrbuf[512];
	} req;
#endif

int main(int argc, char *argv[])
{

	struct rtattr *rta;
	unsigned int mtu = 0;

	if (3 != argc) {
		fprintf(stderr, "usage: %s interface_index mtu\n", argv[0]);
		fprintf(stderr, "must be super user, .e.g root\n"
				"need ip link dev up after excute\n");
		exit(1);
	}

	int rtnetlink_sk = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
	if (-1 == rtnetlink_sk) {
		fprintf(stderr, "socket failure :%s\n", strerror(errno));
		exit(1);
	}

	memset(&req, 0, sizeof(req));
	req.nh.nlmsg_len = NLMSG_LENGTH(sizeof(req.if1));
	req.nh.nlmsg_flags = NLM_F_REQUEST;
	req.nh.nlmsg_type = RTM_NEWLINK;
	req.if1.ifi_family = AF_UNSPEC;
	req.if1.ifi_change = 0xffffffff; /* ??? */
	req.if1.ifi_index = atoi(argv[1]);
	//fprintf(stdout, "index %d\n", req.if1.ifi_index);
	mtu = atoi(argv[2]);
	//fprintf(stdout, "mtu %d\n", mtu);
	rta = (struct rtattr *)(((char *) &req) +
			NLMSG_ALIGN(req.nh.nlmsg_len));
	rta->rta_type = IFLA_MTU;
	rta->rta_len = RTA_LENGTH(sizeof(mtu));
	req.nh.nlmsg_len = NLMSG_ALIGN(req.nh.nlmsg_len) +
	RTA_LENGTH(sizeof(mtu));
	memcpy(RTA_DATA(rta), &mtu, sizeof(mtu));
	if (-1 == send(rtnetlink_sk, &req, req.nh.nlmsg_len, 0)) {
		fprintf(stderr, "send error :%s\n", strerror(errno));
		exit(1);
	}
}
