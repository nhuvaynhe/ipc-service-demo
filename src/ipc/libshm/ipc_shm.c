#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "ipc_shm.h"
#include "mylog.h"

int shm_segment_init(shm_info_t *seg, const char *path, int proj_id, uint64_t size, const eSegmentStatus create_segment) 
{
    int err_no;
    seg->pid = proj_id;
    
    // Generate a key
    seg->key = ftok(path, proj_id);
    if (seg->key == -1) {
        LOG_ERROR("Failed to get key: %s", strerror(errno));
        return -1;
    }

    // Create or get the shared memory segment
    if (create_segment == SEGMENT_CREAT) {
        seg->shm_id = shmget(seg->key, size, IPC_CREAT | 0666);
    }
    // just attach to the segment
    else {
        seg->shm_id = shmget(seg->key, 0, 0666);
    }
    
    if (seg->shm_id == -1) {
        err_no = errno != 0 ? errno : EPERM;
        LOG_ERROR("line: %d, "
                "shmget with key %08x fail, "
                "errno: %d, error info: %s", __LINE__,
                seg->key, err_no, strerror(err_no));
        return -1;
    }

    LOG_INFO("allocate segment success!");
    seg->size = size;

    // Attach to the shared memory segment
    seg->base = (char *)shmat(seg->shm_id, NULL, 0);
    if (seg->base == (char *)-1) {
        LOG_ERROR("Failed to attach: %s", strerror(errno));
        return -1;
    }

    return 0;
}

void shm_segment_detach(shm_info_t *seg) 
{
    if (seg->base) {
        if (shmdt(seg->base) == -1) {
            LOG_ERROR("Failed to detach: %s", strerror(errno));
        } 
        else {
            LOG_INFO("%s detached.", seg->owner);
        }
        seg->base = NULL;
    }
}

void shm_segment_destroy(shm_info_t *seg) 
{
    if (seg->shm_id >= 0) {
        if (shmctl(seg->shm_id, IPC_RMID, NULL) == -1) {
            LOG_ERROR("Failed to destory %d: %s", seg->shm_id, strerror(errno));
        }
        else {
            LOG_INFO("%s destroyed.", seg->owner);
        }
        seg->shm_id = -1;
    }
}
