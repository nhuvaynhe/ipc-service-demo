#ifndef WIFI_SCAN_H
#define WIFI_SCAN_H

#include <semaphore.h>
#include <time.h>

#define MAX_SSID_LEN    64
#define MAX_BSS_LEN     32
#define MAX_LINE_LEN    100

#define MAX_WIFI_NETWORKS    20

typedef struct {
    char bss[MAX_BSS_LEN];
    char ssid[MAX_SSID_LEN];
} WifiNetwork_t;

typedef struct {
    // WifiNetwork_t *networks;
    WifiNetwork_t networks[MAX_WIFI_NETWORKS];
    int count;
} WifiScanResult_t;

typedef struct {
    sem_t start_scan;           // Main → Wi-Fi: Start scan
    sem_t scan_done;            // Wi-Fi → Main: Scan complete
    WifiScanResult_t wifi_list;
    time_t last_heartbeat;
} WifiService_t;


/**
 * scan nearby Wi-Fi networks
 * parameters:
 *   wifi_list : output scan result structure
 *   interface : network interface to scan (e.g., "wlan0")
 */
void scan_wifi_networks(WifiScanResult_t *wifi_list, const char *interface);

/**
 * get array of SSID strings from scan result
 * parameters:
 *   result : scan result from scan_wifi_networks
 * return:
 *   NULL-terminated array of SSID strings
 *   caller must not modify or free the returned strings
 */
char **get_wifi_ssids(WifiScanResult_t *result);


#endif // WIFI_SCAN_H
