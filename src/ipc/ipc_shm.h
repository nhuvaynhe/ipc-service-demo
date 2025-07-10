#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

/* @page
 * https://www.sao.ru/hq/sts/linux/doc/ipc_guide/shmem.html
 *
 */

typedef enum {
    SEGMENT_ATTACH,
    SEGMENT_CREAT,
} eSegmentStatus;

/* 
 * @copyright https://github.com/happyfish100/libshmcache
 *
 */
typedef struct {
    char *owner;       // Project name
    int pid;           // Project ID used with ftok
    key_t key;         // System V shared memory key
    int shm_id;        // Shared memory ID from shmget
    uint64_t size;     // Allocated shared memory size
    char *base;        // Mapped address from shmat
} shm_info_t;



/**
 * initialize a shared memory segment
 * parameters:
 *   seg            : output shared memory info
 *   path           : file path used for ftok
 *   proj_id        : project id for key generation
 *   size           : shared memory size in bytes
 *   create_segment : create or just attach to the mmap
 * return 0 on success, < 0 on failure
 */
int shm_segment_init(shm_info_t *seg, const char *path, int proj_id, uint64_t size, const eSegmentStatus create_segment);

/**
 * detach the shared memory segment
 * parameters:
 *   seg : shared memory info to detach
 */
void shm_segment_detach(shm_info_t *seg);

/**
 * destroy the shared memory segment
 * parameters:
 *   seg : shared memory info to destroy
 * note: memory will be removed after all detach
 */
void shm_segment_destroy(shm_info_t *seg);