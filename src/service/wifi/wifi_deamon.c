#include <string.h>
#include <pthread.h>

#include "aw_wifi.h"
#include "ipc_shm.h"
#include "mylog.h"

static void *heartbeat_thread(void *arg) {
    WifiService_t *wifi_serv = (WifiService_t *)arg;

    while (1) {
        wifi_serv->last_heartbeat = time(NULL);
        sleep(5);
    }
}

int main(void)
{
    LOG_INFO("%s wake up", __FILENAME__);

    shm_info_t shm = {
        .owner = "wifi"
    };

    if (shm_segment_init(&shm, "/tmp", 'W', sizeof(WifiService_t), SEGMENT_CREAT)) {
        return 1;
    }

    WifiService_t *wifi_serv = (&shm)->base;

    sem_init(&(wifi_serv->start_scan), 1, 0);
    sem_init(&(wifi_serv->scan_done), 1, 0);

    pthread_t hb_thread;
    pthread_create(&hb_thread, NULL, heartbeat_thread, wifi_serv);

    while(1) 
    {
        LOG_INFO("%s waiting...", __FILENAME__, wifi_serv->last_heartbeat);

        // Wait for main to signal scan
        sem_wait(&(wifi_serv->start_scan));

        // Perform scan (mock example)
        LOG_INFO("%s scanning...", __FILENAME__);
        sleep(2);

        scan_wifi_networks(&(wifi_serv->wifi_list), "wlp1s0");

        // Signal scan complete
        sem_post(&(wifi_serv->scan_done));
    }
}