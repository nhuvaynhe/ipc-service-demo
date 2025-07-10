#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "aw_wifi.h"
#include "mylog.h"


void scan_wifi_networks(WifiScanResult_t *wifi_list, const char *interface) {
    FILE *fp;
    char line[MAX_LINE_LEN];
    char current_bss[MAX_BSS_LEN] = {0};
    int total_wifi = 0;

    const char *password = "password";  // <-- replace your password here

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "echo \"%s\" | sudo -S iw %s scan | grep -E 'SSID|BSS'", password, interface);
    LOG_INFO("cmd: %s", cmd);

    fp = popen(cmd, "r");
    if (!fp) {
        LOG_ERROR("popen");
        wifi_list->count = 0;
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        // Trim leading whitespace
        char *ptr = line;
        while (*ptr == ' ' || *ptr == '\t') ptr++;

        // Parse BSS
        if (strncmp(ptr, "BSS ", 4) == 0) {
            sscanf(ptr, "BSS %31[^ ]", current_bss);
        }
        // Parse SSID
        else if (strncmp(ptr, "SSID:", 5) == 0) {
            if (total_wifi < MAX_WIFI_NETWORKS) {
                strncpy(wifi_list->networks[total_wifi].bss, current_bss, MAX_BSS_LEN - 1);
                wifi_list->networks[total_wifi].bss[MAX_BSS_LEN - 1] = '\0';

                strncpy(wifi_list->networks[total_wifi].ssid, ptr + 5, MAX_SSID_LEN - 1);
                wifi_list->networks[total_wifi].ssid[MAX_SSID_LEN - 1] = '\0';

                // Remove newline from SSID
                int newline_index = strcspn(wifi_list->networks[total_wifi].ssid, "\n");
                wifi_list->networks[total_wifi].ssid[newline_index] = '\0';

                total_wifi++;
            }
        }
    }

    pclose(fp);

    wifi_list->count = total_wifi;
}

char **get_wifi_ssids(WifiScanResult_t *result) {
    if (!result || result->count == 0 || !result->networks) {
        return NULL;
    }

    // Allocate array of string pointers
    char **ssids = (char **)malloc(result->count * sizeof(char *));
    if (!ssids) {
        LOG_ERROR("malloc");
        return NULL;
    }

    // Allocate and copy each SSID
    for (int i = 0; i < result->count; i++) {
        size_t ssid_len = strlen(result->networks[i].ssid) + 1;
        ssids[i] = (char *)malloc(ssid_len);

        if (ssids[i]) {
            strncpy(ssids[i], result->networks[i].ssid, ssid_len);
            ssids[i][ssid_len - 1] = '\0';
        } 
        // Clean up when malloc failed
        else {
            for (int j = 0; j < i; j++) {
                free(ssids[j]);
            }
            free(ssids);
            LOG_ERROR("malloc");
            return NULL;
        }
    }

    return ssids;
}