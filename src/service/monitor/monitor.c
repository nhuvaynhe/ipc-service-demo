#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "ipc_shm.h"
#include "aw_wifi.h"
#include "mylog.h"

#define TIMEOUT_SECONDS 3

int main() {
    LOG_INFO("%s wake up", __FILENAME__);

    shm_info_t shm = {
        .owner = "monitor"
    };

    if (shm_segment_init(&shm, "/tmp", 'W', sizeof(WifiService_t), SEGMENT_ATTACH) != 0) {
        return 1;
    }

    WifiService_t *wifi_serv = (&shm)->base;

    while (1) {
        time_t now = time(NULL);
        double diff = difftime(now, wifi_serv->last_heartbeat);

        if (diff > TIMEOUT_SECONDS) {
            LOG_INFO("[%s] wifi_deamon sleep... [%d] \n", __FILENAME__, diff);
        } else {
            LOG_INFO("[%s] wifi_deamon is awake [%d]\n", __FILENAME__, diff);
        }

        sleep(2);
    }

    shm_segment_detach(&shm);
    return 0;
}
