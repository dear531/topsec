#if 0
安装依赖库：
apt install libnetfilter-log-dev libnfnetlink-dev

编译方法：
gcc nflog.c -lnetfilter_log
#endif

#include <stdio.h>
#include <netinet/in.h>
#include <libnetfilter_log/libnetfilter_log.h>
#include <unistd.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#if 0
       #include <stddef.h>
       #include <libnetfilter_log/libnetfilter_log.h>

       struct nfulnl_msg_packet_hdr * nflog_get_msg_packet_hdr (struct nflog_data *nfad)
       uint16_t nflog_get_hwtype (struct nflog_data *nfad)
       uint16_t nflog_get_msg_packet_hwhdrlen (struct nflog_data *nfad)
       char * nflog_get_msg_packet_hwhdr (struct nflog_data *nfad)
       uint32_t nflog_get_nfmark (struct nflog_data *nfad)
       int nflog_get_timestamp (struct nflog_data *nfad, struct timeval *tv)
       uint32_t nflog_get_indev (struct nflog_data *nfad)
       uint32_t nflog_get_physindev (struct nflog_data *nfad)
       uint32_t nflog_get_outdev (struct nflog_data *nfad)
       uint32_t nflog_get_physoutdev (struct nflog_data *nfad)
       struct nfulnl_msg_packet_hw * nflog_get_packet_hw (struct nflog_data *nfad)
       int nflog_get_payload (struct nflog_data *nfad, char **data)
       char * nflog_get_prefix (struct nflog_data *nfad)
       int nflog_get_uid (struct nflog_data *nfad, uint32_t *uid)
       int nflog_get_gid (struct nflog_data *nfad, uint32_t *gid)
       int nflog_get_seq (struct nflog_data *nfad, uint32_t *seq)
       int nflog_get_seq_global (struct nflog_data *nfad, uint32_t *seq)
       int nflog_get_ctid (struct nflog_data *nfad, uint32_t *id)
#endif

int cb(struct nflog_g_handle *gh, struct nfgenmsg *nfmsg,
			    struct nflog_data *nfad, void *data)
{
	uint32_t indev = nflog_get_indev (nfad);
	fprintf(stdout, "indev :%u\n", indev);
#if 0
struct nfulnl_msg_packet_hw {
	u_int16_t	hw_addrlen;
	u_int16_t	_pad;
	u_int8_t	hw_addr[8];
} __attribute__ ((packed));
#endif
	struct nfulnl_msg_packet_hw *hw = nflog_get_packet_hw (nfad);
	int i = 0;
	unsigned short addr_len = ntohs(hw->hw_addrlen);
	for (i = 0; i < addr_len; i++) {
		if (addr_len - 1 == i) {
			fprintf(stdout, "%02x\n", hw->hw_addr[i]);
		} else {
			fprintf(stdout, "%02x:", hw->hw_addr[i]); }
	}
	char *payload = NULL;
	int payload_len = nflog_get_payload (nfad, &payload);
	fprintf(stdout, "%d\n", payload_len);
	if (NULL == payload) {
		fprintf(stderr, "payload is %p\n", payload);
		goto end;
	}
	
#if 0
struct iphdr
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
  };
#endif
	struct iphdr *iph = (void*)payload;
	if (IPPROTO_TCP == iph->protocol) {
		fprintf(stdout, "iph->protocol :%u\n", iph->protocol);
	} else {
		goto end;
	}
	struct tcphdr *tcph =(void *)(((void *)iph) + iph->ihl * 4);
	fprintf(stdout, "tcph->source:%d, tcph->dest:%d\n", ntohs(tcph->source), ntohs(tcph->dest));
#if 0
struct tcphdr
  {
    __extension__ union
    {
      struct
      {
	uint16_t th_sport;	/* source port */
	uint16_t th_dport;	/* destination port */
	tcp_seq th_seq;		/* sequence number */
	tcp_seq th_ack;		/* acknowledgement number */
# if __BYTE_ORDER == __LITTLE_ENDIAN
	uint8_t th_x2:4;	/* (unused) */
	uint8_t th_off:4;	/* data offset */
# endif
# if __BYTE_ORDER == __BIG_ENDIAN
	uint8_t th_off:4;	/* data offset */
	uint8_t th_x2:4;	/* (unused) */
# endif
	uint8_t th_flags;
# define TH_FIN	0x01
# define TH_SYN	0x02
# define TH_RST	0x04
# define TH_PUSH	0x08
# define TH_ACK	0x10
# define TH_URG	0x20
	uint16_t th_win;	/* window */
	uint16_t th_sum;	/* checksum */
	uint16_t th_urp;	/* urgent pointer */
      };
      struct
      {
	uint16_t source;
	uint16_t dest;
	uint32_t seq;
	uint32_t ack_seq;
# if __BYTE_ORDER == __LITTLE_ENDIAN
	uint16_t res1:4;
	uint16_t doff:4;
	uint16_t fin:1;
	uint16_t syn:1;
	uint16_t rst:1;
	uint16_t psh:1;
	uint16_t ack:1;
	uint16_t urg:1;
	uint16_t res2:2;
# elif __BYTE_ORDER == __BIG_ENDIAN
	uint16_t doff:4;
	uint16_t res1:4;
	uint16_t res2:2;
	uint16_t urg:1;
	uint16_t ack:1;
	uint16_t psh:1;
	uint16_t rst:1;
	uint16_t syn:1;
	uint16_t fin:1;
# else
#  error "Adjust your <bits/endian.h> defines"
# endif
	uint16_t window;
	uint16_t check;
	uint16_t urg_ptr;
      };
    };
};
#endif
end:
	return 0;
}

int main(void)
{
#if 0
       struct nflog_handle * nflog_open (void)
       int nflog_close (struct nflog_handle *h)
       int nflog_bind_pf (struct nflog_handle *h, uint16_t pf)
       int nflog_unbind_pf (struct nflog_handle *h, uint16_t pf)
#endif
	struct nflog_handle *h = nflog_open();
#if 0
	fprintf(stdout, "h :%p\n", h);
#endif
	if (NULL == h) {
		fprintf(stderr, "nflog_open error :%m\n");
		 goto error;
	}

	if (-1 == nflog_bind_pf(h, AF_INET)) {
		fprintf(stderr, "nflog_bind_pf error :%m\n");
		goto nflog_open_error;
	}
#if 0
       int nflog_fd (struct nflog_handle *h)
       int nflog_callback_register (struct nflog_g_handle *gh, nflog_callback *cb, void *data)
       int nflog_handle_packet (struct nflog_handle *h, char *buf, int len)
       struct nflog_g_handle * nflog_bind_group (struct nflog_handle *h, uint16_t num)
       int nflog_unbind_group (struct nflog_g_handle *gh)
       int nflog_set_mode (struct nflog_g_handle *gh, uint8_t mode, uint32_t range)
       int nflog_set_timeout (struct nflog_g_handle *gh, uint32_t timeout)
       int nflog_set_qthresh (struct nflog_g_handle *gh, uint32_t qthresh)
       int nflog_set_nlbufsiz (struct nflog_g_handle *gh, uint32_t nlbufsiz)
       int nflog_set_flags (struct nflog_g_handle *gh, uint16_t flags)
#endif
	struct nflog_g_handle *gh = nflog_bind_group(h, 100);
	if (NULL == gh) {
		fprintf(stderr, "nflog_bind_group error :%m\n");
		goto bind_pf_error;
	}

	if (nflog_set_mode(gh, NFULNL_COPY_PACKET, 0xffff) < 0) {
		fprintf(stderr, "nflog_set_mode error :%m\n");
		goto bind_group_error;
	}

	if (-1 == nflog_callback_register(gh, cb, NULL)) {
		fprintf(stderr, "nflog_callback_register :%m\n");
		goto nflog_fd_error;
	}

#if 0

               fd = nflog_fd(h);

               while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {
                       printf("pkt received (len=%u)\n", rv);
                       nflog_handle_packet(h, buf, rv);
               }
#endif
	int fd = nflog_fd(h);
	if (-1 == fd) {
		fprintf(stderr, "nflog_fd error :%m\n");
		goto bind_group_error;
	}

	int rv = 0;
	char buff[4096] = {};
	while ((rv = recv(fd, buff, sizeof(buff), 0)) && rv >= 0) {
		//fprintf(stdout, "111\n");
		nflog_handle_packet(h, buff, rv);
	}

nflog_fd_error:
	if (0 <= fd) {
		close(fd);
		fd = -1;
	}


bind_group_error:
	if (NULL != gh && -1 == nflog_unbind_group (gh)) {
		fprintf(stderr, "nflog_unbind_group error :%m\n");
		goto error;
	}

bind_pf_error:
	if (-1 == nflog_unbind_pf (h, PF_INET)) {
		fprintf(stderr, "nflog_unbind_pf error :%m\n");
		goto error;
	}
nflog_open_error:

	if (NULL != h) {
		nflog_close(h);
		h = NULL;
	}
error:
	
	return 0;
}
