#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>     // select
#include <sys/time.h>   // gettimeofday
#include "packet.h"

// long long get_current_time() {
//     struct timeval time = {};
//     if (gettimeofday(&time, NULL) == -1) {
//         printf("gettimeofday failed, errno = %s (%d)\n", strerror(errno), errno);
//         return -1;
//     }
//     return (long long) time.tv_sec * 1000 + (long long) time.tv_usec / 1000;
// }



int main() {

    disctx ctx = {
        .achost = "",
        .devid = "11:22:33:44:55:77",
        .product = "ap001",
    };

    dis_socket_create(&ctx);

    // lssdp_ctx lssdp = {
    //     .port = 18899,
    //     // .debug = true,           // debug

    //     // callback
    //     //.network_interface_changed_callback = show_interface_list_and_rebind_socket,
    //     .packet_received_callback           = show_ssdp_packet
    // };

    /* get network interface at first time, network_interface_changed_callback will be invoke
     * SSDP socket will be created in callback function
     */
    // lssdp_network_interface_update(&lssdp);
    // lssdp_socket_create(&lssdp);

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
        if (current_time - last_time >= 5000) {
            //lssdp_network_interface_update(&lssdp); // update network interface
            last_time = current_time;               // update last_time
        }
    }

    return EXIT_SUCCESS;
}
