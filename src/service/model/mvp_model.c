#include <stdio.h>
#include <unistd.h>

#include "mylog.h"
#include "aw_wifi.h"
#include "ipc_shm.h"

int main() 
{
    LOG_INFO("%s wake up", __FILENAME__);

    shm_info_t shm = { .owner = "main" };
    if (shm_segment_init(&shm, "/tmp", 'W', sizeof(WifiService_t), SEGMENT_ATTACH) != 0) {
        return 1;
    }

    WifiService_t *wifi_serv = (&shm)->base;

    LOG_INFO("%s request scan...", __FILENAME__);

    sem_post(&(wifi_serv->start_scan));
    sem_wait(&(wifi_serv->scan_done));

    const WifiScanResult_t wifi_list = wifi_serv->wifi_list;
    LOG_INFO("%s wifi sucess, get %d scan...", __FILENAME__, wifi_list.count);

    for (int i = 0; i < wifi_list.count; ++i) {
        printf("    [%d] SSID: %s\n", i+1, wifi_list.networks[i].ssid);
    }

    shm_segment_detach(&shm);
    // shm_segment_destroy(&shm);
    return 0;
}
