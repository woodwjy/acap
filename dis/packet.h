

#define DIS_DEVID_LEN 32
#define DIS_PRODUCT_LEN 32
#define DIS_BUFFER_LEN   256
#define DIS_STAGE_LEN 16
#define DIS_HOST_LEN 64
#define DIS_IP_LEN   16

#define STAGE_DISCOVERY "discovery"
#define STAGE_OFFER "offer"
#define STAGE_READY "ready"
#define STAGE_ACK "ack"

#define ADDR_MULTICAST "239.255.255.250"
#define PORT_MULTICAST 8899


typedef struct disctx {
    int             sock;                                   // socket
    char        achost[DIS_HOST_LEN];
    char        devid     [DIS_DEVID_LEN];
    char        product [DIS_PRODUCT_LEN];
} disctx;


typedef struct dis_packet {
    char            stage   [DIS_STAGE_LEN]; 
    char            achost  [DIS_HOST_LEN];
    char            product [DIS_PRODUCT_LEN];
    char            devid   [DIS_DEVID_LEN];
} dis_packet;


int dis_socket_read(disctx *ctx);
int send_multicast_data(const char * data, unsigned short port);
int dis_packet_parser(const char * data, size_t data_len, dis_packet * packet);
int dis_muti_send_discovery(disctx * ctx);
int dis_muti_send_ready(disctx * ctx);
int dis_socket_create(disctx *ctx);
int dis_socket_close(disctx * ctx);

long long get_current_time();