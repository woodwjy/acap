

#define DIS_DEVID_LEN 32
#define DIS_PRODUCT_LEN 32
#define DIS_BUFFER_LEN   256
#define DIS_STAGE_LEN 16
#define DIS_HOST_LEN 64
#define DIS_IP_LEN   16

typedef struct disctx {
    int             sock;                                   // socket
    char        achost[DIS_HOST_LEN];
    char        devid     [DIS_DEVID_LEN];
    char        product [DIS_PRODUCT_LEN];
    int (* done_callback)     (struct disctx * ctx);
} disctx;


typedef struct dis_packet {
    char            stage   [DIS_STAGE_LEN]; 
    char            achost  [DIS_HOST_LEN];
    char            product [DIS_PRODUCT_LEN];
    char            devid   [DIS_DEVID_LEN];
} dis_packet;


int dis_socket_read(disctx *ctx);
int dis_socket_create(disctx *ctx);
int dis_socket_close(disctx * ctx);

long long get_current_time();

