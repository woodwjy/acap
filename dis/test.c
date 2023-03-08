#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>     // select
#include <sys/time.h>   // gettimeofday
#include "packet.h"
#include "yyjson.h"

void test_send_multicast_data(char *arg){
    int ret = send_multicast_data(arg, 18899);
    if (ret == 0){
        printf("send ok\n");
    }
}

void test_dis_packet_parser(){
    dis_packet packet = {};
    const char *data = "{\"stage\":\"ready\",\"achost\":\"192.168.0.1:1883\",\"product\":\"ac001\", \"devid\":\"11:22:33:44:55:66\"}";
    int ret = dis_packet_parser(data, strlen(data), &packet);
    if(ret == 0 ){
        printf("%s\n", packet.stage);
        printf("%s\n",packet.achost);
        printf("%s\n",packet.product);
        printf("%s\n",packet.devid);
    }
}

void test_dis_muti_send_discovery(){

    disctx ctx = {
        .sock = 1,
        .achost = "192.168.0.1:1883",
        .devid = "11L22:33:44:55:66",
        .product = "ac001",
    };

    int ret = dis_muti_send_discovery(&ctx);
    if(ret != 0){

    }

}


int bz(){
    disctx ctx = {
        .sock = 1,
        .achost = "192.168.0.1:1883",
        .devid = "11L22:33:44:55:66",
        .product = "ac001",
    };

    long long last_time = get_current_time();
    if (last_time < 0) {
        printf("got invalid timestamp %lld\n", last_time);
        return EXIT_SUCCESS;
    }


    // Main Loop
    for (;;) {
        fd_set fs;
        FD_ZERO(&fs);
        FD_SET(ctx.sock, &fs);
        struct timeval tv = {
            .tv_usec = 500 * 1000   // 500 ms
        };

        int ret = select(ctx.sock + 1, &fs, NULL, NULL, &tv);
        if (ret < 0) {
            printf("select error, ret = %d\n", ret);
            break;
        }

        if (ret > 0) {
            dis_socket_read(&ctx);
        }

        // get current time
        long long current_time = get_current_time();
        if (current_time < 0) {
            printf("got invalid timestamp %lld\n", current_time);
            break;
        }

        // doing task per 5 seconds
        if (current_time - last_time >= 3000) {
            dis_muti_send_ready(&ctx);
            last_time = current_time;               // update last_time
        }
    }

    return EXIT_SUCCESS;
}

void main(int argc, char **argv){
    // test_dis_packet_parser();
    // test_dis_muti_send_discovery();
    // bz();
    test_send_multicast_data(argv[1]);
}