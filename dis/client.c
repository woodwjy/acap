#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>     // select
#include <sys/time.h>   // gettimeofday
#include "packet.h"
#include "yyjson.h"


int loop(disctx *ctx){

    long long last_time = get_current_time();
    if (last_time < 0) {
        printf("got invalid timestamp %lld\n", last_time);
        return EXIT_SUCCESS;
    }

    // Main Loop
    for (;;) {
        fd_set fs;
        FD_ZERO(&fs);
        FD_SET(ctx->sock, &fs);
        struct timeval tv = {
            .tv_usec = 500 * 1000   // 500 ms
        };

        int ret = select(ctx->sock + 1, &fs, NULL, NULL, &tv);
        if (ret < 0) {
            printf("select error, ret = %d\n", ret);
            break;
        }

         printf("xxx1\n");

        if (ret > 0) {
            printf("xxx\n");
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
            // dis_muti_send_ready(&ctx);
            printf("socket %d\n", ctx->sock);
            last_time = current_time;               // update last_time
        }
    }

    return EXIT_SUCCESS;
}

void main(){
    disctx ctx = {
        .achost = "",
        .devid = "11L22:33:44:55:77",
        .product = "ap001",
    };

    dis_socket_create(&ctx);
     printf("socket %d\n", ctx.sock);
    loop(&ctx);
}