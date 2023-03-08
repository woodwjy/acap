#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include "packet.h"
#include "yyjson.h"

#define TIMES_ON_UP 10

int ac_loop(disctx ctx){
    long long last_time = get_current_time();
    if (last_time < 0) {
        printf("got invalid timestamp %lld\n", last_time);
        return EXIT_SUCCESS;
    }

    static int times = TIMES_ON_UP;
    
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

        // 获取当前时间
        long long current_time = get_current_time();
        if (current_time < 0) {
            printf("got invalid timestamp %lld\n", current_time);
            break;
        }

        // 程序启动后每5秒发送一次ready报文，总共发10次
        if (current_time - last_time >= 5000) {
            if(times-- > 0){
                dis_muti_send_ready(&ctx);
            }
            last_time = current_time;
        }
    }

    return EXIT_SUCCESS;
}

void main(){
    // AC的参数设置
    disctx ctx = {
        .achost = "192.168.0.1:1883",
        .devid = "11L22:33:44:55:66",
        .product = "ac001",
    };

    dis_socket_create(&ctx);
    ac_loop(ctx);
}