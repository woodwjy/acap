#include <stdio.h>      // snprintf, vsnprintf
#include <stdlib.h>     // malloc, free
#include <stdarg.h>     // va_start, va_end, va_list
#include <string.h>     // memset, memcpy, strlen, strcpy, strcmp, strncasecmp, strerror
#include <errno.h>      // errno
#include <unistd.h>     // close
#include <sys/time.h>   // gettimeofday
#include <sys/ioctl.h>  // ioctl, FIONBIO
#include <net/if.h>     // struct ifconf, struct ifreq
#include <fcntl.h>      // fcntl, F_GETFD, F_SETFD, FD_CLOEXEC
#include <sys/socket.h> // struct sockaddr, AF_INET, SOL_SOCKET, socklen_t, setsockopt, socket, bind, sendto, recvfrom
#include <netinet/in.h> // struct sockaddr_in, struct ip_mreq, INADDR_ANY, IPPROTO_IP, also include <sys/socket.h>
#include <arpa/inet.h>  // inet_aton, inet_ntop, inet_addr, also include <netinet/in.h>
#include "packet.h"
#include "yyjson.h"

static struct {
    const char * MULTICAST;
    const char * STAGE;
    const char * AC_HOST;
    const char * PRODUCT;
    const char * DEV_ID;
} Global = {
    .MULTICAST  = "239.255.255.250",
    .STAGE   = "stage",
    .AC_HOST = "achost",
    .PRODUCT = "product",
    .DEV_ID = "devid"
};


int send_multicast_data(const char * data, unsigned short port) {
    if (data == NULL) {
        printf("data should not be NULL\n");
        return -1;
    }

    size_t data_len = strlen(data);
    if (data_len == 0) {
        printf("data length should not be empty\n");
        return -1;
    }

    int result = -1;

    // 1. create UDP socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        printf("create socket failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // 2. bind socket
    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY
    };
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("bind failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // 3. disable IP_MULTICAST_LOOP
    char opt = 0;
    if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &opt, sizeof(opt)) < 0) {
        printf("setsockopt IP_MULTICAST_LOOP failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // 4. set destination address
    struct sockaddr_in dest_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    if (inet_aton(ADDR_MULTICAST, &dest_addr.sin_addr) == 0) {
        printf("inet_aton failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // 5. send data
    if (sendto(fd, data, data_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1) {
        printf("sendto failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    result = 0;

end:
    if (fd >= 0 && close(fd) != 0) {
        printf("close fd %d failed, errno = %s (%d)\n", strerror(errno), errno);
    }
    return result;
}


int dis_packet_parser(const char * data, size_t data_len, dis_packet * packet){
    if (data == NULL) {
        printf("data should not be NULL\n");
        return -1;
    }

    if (data_len != strlen(data)) {
        printf("data_len (%zu) is not match to the data length (%zu)\n", data_len, strlen(data));
        return -1;
    }

    if (packet == NULL) {
        printf("packet should not be NULL\n");
        return -1;
    }


    yyjson_doc *doc = yyjson_read(data, strlen(data), 0);
    yyjson_val *root = yyjson_doc_get_root(doc);

    yyjson_val *stage = yyjson_obj_get(root, Global.STAGE);
    snprintf(packet->stage, DIS_STAGE_LEN, "%s", yyjson_get_str(stage));

    yyjson_val *achost = yyjson_obj_get(root, Global.AC_HOST);
    snprintf(packet->achost, DIS_HOST_LEN, "%s", yyjson_get_str(achost));


    yyjson_val *product = yyjson_obj_get(root, Global.PRODUCT);
    snprintf(packet->product, DIS_PRODUCT_LEN, "%s", yyjson_get_str(product));

    yyjson_val *devid = yyjson_obj_get(root, Global.DEV_ID);
    snprintf(packet->devid, DIS_DEVID_LEN, "%s", yyjson_get_str(devid));

    yyjson_doc_free(doc);

    return 0;
}

int dis_muti_send_discovery(disctx * ctx){

    if (ctx == NULL) {
        printf("lssdp should not be NULL\n");
        return -1;
    }

    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_str(doc, root, Global.STAGE, STAGE_DISCOVERY);
    yyjson_mut_obj_add_str(doc, root, Global.AC_HOST, "");
    yyjson_mut_obj_add_str(doc, root, Global.PRODUCT, ctx->product);
    yyjson_mut_obj_add_str(doc, root, Global.DEV_ID, ctx->devid);

    const char *json = yyjson_mut_write(doc, 0, NULL);

    char buffer[DIS_BUFFER_LEN] = {};
    snprintf(buffer, sizeof(buffer),"%s", json);

    int ret = send_multicast_data(buffer, PORT_MULTICAST);
    if (ret != 0){
        printf("send fail\n");
    }

    printf(buffer);

    return 0;
}

int dis_muti_send_ready(disctx * ctx){

    if (ctx == NULL) {
        printf("lssdp should not be NULL\n");
        return -1;
    }

    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_str(doc, root, Global.STAGE, STAGE_READY);
    yyjson_mut_obj_add_str(doc, root, Global.AC_HOST, ctx->achost);
    yyjson_mut_obj_add_str(doc, root, Global.PRODUCT, ctx->product);
    yyjson_mut_obj_add_str(doc, root, Global.DEV_ID, ctx->devid);

    const char *json = yyjson_mut_write(doc, 0, NULL);

    char buffer[DIS_BUFFER_LEN] = {};
    snprintf(buffer, sizeof(buffer),"%s", json);

    int ret = send_multicast_data(buffer, PORT_MULTICAST);
    if (ret != 0){
        printf("send fail\n");
    }

    printf(buffer);

    return 0;

}


static int dis_send_offer(disctx * ctx, struct sockaddr_in address){

    // peer address
    char peerip[DIS_IP_LEN]={};
    if (inet_ntop(AF_INET, &address.sin_addr, peerip, sizeof(peerip)) == NULL) {
        printf("inet_ntop failed, errno = %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    // address.sin_port = htons(PORT_MULTICAST);

    // payload
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_str(doc, root, Global.STAGE, STAGE_OFFER);
    yyjson_mut_obj_add_str(doc, root, Global.AC_HOST, ctx->achost);
    yyjson_mut_obj_add_str(doc, root, Global.PRODUCT, ctx->product);
    yyjson_mut_obj_add_str(doc, root, Global.DEV_ID, ctx->devid);

    const char *json = yyjson_mut_write(doc, 0, NULL);

    char buffer[DIS_BUFFER_LEN] = {};
    int len = snprintf(buffer, sizeof(buffer),"%s", json);


    // send response
    if (sendto(ctx->sock, buffer, len, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1) {
        printf("send RESPONSE to %s failed, errno = %s (%d)\n", peerip, strerror(errno), errno);
        return -1;
    }

    return 0;
}

static int dis_send_ack(disctx * ctx, struct sockaddr_in address){
    // peer address
    char peerip[DIS_IP_LEN]={};
    if (inet_ntop(AF_INET, &address.sin_addr, peerip, sizeof(peerip)) == NULL) {
        printf("inet_ntop failed, errno = %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    // 
    // address.sin_port = htons(PORT_MULTICAST);

    // payload
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);
    yyjson_mut_obj_add_str(doc, root, Global.STAGE, STAGE_ACK);
    // yyjson_mut_obj_add_str(doc, root, Global.AC_HOST, "");
    yyjson_mut_obj_add_str(doc, root, Global.PRODUCT, ctx->product);
    yyjson_mut_obj_add_str(doc, root, Global.DEV_ID, ctx->devid);

    const char *json = yyjson_mut_write(doc, 0, NULL);

    char buffer[DIS_BUFFER_LEN] = {};
    int len = snprintf(buffer, sizeof(buffer),"%s", json);


    // send response
    if (sendto(ctx->sock, buffer, len, 0, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) == -1) {
        printf("send RESPONSE to %s failed, errno = %s (%d)\n", peerip, strerror(errno), errno);
        return -1;
    }

    return 0;
}


int dis_socket_read(disctx *ctx){
    if (ctx == NULL) {
        printf("lssdp should not be NULL\n");
        return -1;
    }

    if (ctx->sock <= 0) {
        printf("DIS socket (%d) has not been setup.\n", ctx->sock);
        return -1;
    }

    char buffer[DIS_BUFFER_LEN] = {};
    struct sockaddr_in address = {};
    socklen_t address_len = sizeof(struct sockaddr_in);


    ssize_t recv_len = recvfrom(ctx->sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, &address_len);
    if (recv_len == -1) {
        printf("recvfrom fd %d failed, errno = %s (%d)\n", ctx->sock, strerror(errno), errno);
        return -1;
    }

    printf("rcv buffer:%s\n", buffer);

    dis_packet packet = {};
    if (dis_packet_parser(buffer, recv_len, &packet) != 0) {
        goto end;
    }

    if(strcmp(packet.stage, STAGE_DISCOVERY) == 0){
        dis_send_offer(ctx, address);
        goto end;
    }

    if(strcmp(packet.stage, STAGE_READY) == 0){
        dis_send_ack(ctx, address);
        goto end;
    }

    if(strcmp(packet.stage, STAGE_OFFER) == 0){
        dis_send_ack(ctx, address);
        goto end;
    }

end:
    return 0;
}


long long get_current_time() {
    struct timeval time = {};
    if (gettimeofday(&time, NULL) == -1) {
        printf("gettimeofday failed, errno = %s (%d)\n", strerror(errno), errno);
        return -1;
    }
    return (long long) time.tv_sec * 1000 + (long long) time.tv_usec / 1000;
}

int dis_socket_close(disctx * ctx){
    if (ctx == NULL) {
        printf("lssdp should not be NULL\n");
        return -1;
    }

    // check lssdp->sock
    if (ctx->sock <= 0) {
        printf("SSDP socket is %d, ignore socket_close request.\n", ctx->sock);
        return -1;
    }

    // close socket
    if (close(ctx->sock) != 0) {
        printf("close socket %d failed, errno = %s (%d)\n", ctx->sock, strerror(errno), errno);
        return -1;
    };


    return 0;
}

int dis_socket_create(disctx *ctx){
    if (ctx == NULL) {
        printf("ctx should not be NULL\n");
        return -1;
    }

    // close original socket
    dis_socket_close(ctx);

    // create udp socket
    ctx->sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (ctx->sock < 0) {
        printf("create socket failed, errno = %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    int result = -1;


    // set non-blocking
    int opt = 1;
    if (ioctl(ctx->sock, FIONBIO, &opt) != 0) {
        printf("ioctl FIONBIO failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // set reuse address
    if (setsockopt(ctx->sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        printf("setsockopt SO_REUSEADDR failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // set FD_CLOEXEC (http://kaivy2001.pixnet.net/blog/post/32726732)
    int sock_opt = fcntl(ctx->sock, F_GETFD);
    if (sock_opt == -1) {
        printf("fcntl F_GETFD failed, errno = %s (%d)\n", strerror(errno), errno);
    } else {
        // F_SETFD
        if (fcntl(ctx->sock, F_SETFD, sock_opt | FD_CLOEXEC) == -1) {
            printf("fcntl F_SETFD FD_CLOEXEC failed, errno = %s (%d)\n", strerror(errno), errno);
        }
    }

    // bind socket
    struct sockaddr_in addr = {
        .sin_family      = AF_INET,
        .sin_port        = PORT_MULTICAST,
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };
    if (bind(ctx->sock, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        printf("bind failed, errno = %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    // set IP_ADD_MEMBERSHIP
    struct ip_mreq imr = {
        .imr_multiaddr.s_addr = inet_addr(ADDR_MULTICAST),
        .imr_interface.s_addr = htonl(INADDR_ANY)
    };
    if (setsockopt(ctx->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(struct ip_mreq)) != 0) {
        printf("setsockopt IP_ADD_MEMBERSHIP failed: %s (%d)\n", strerror(errno), errno);
        goto end;
    }

    printf("create SSDP socket %d\n", ctx->sock);
    result = 0;
end:
    if (result == -1) {
        dis_socket_close(ctx);
    }
    return result;
}