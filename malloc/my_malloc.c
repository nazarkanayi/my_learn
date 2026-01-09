#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

/* -------------- Configuration -------------- */
#define ALIGNMENT 16
#define MIN_SPLIT_SIZE 32   /* minimum size of leftover block after split */
#define MAGIC 0xDEADBEEFCAFEBABEULL

/* -------------- Helper macros -------------- */
#define ALIGN_UP(x, a) (((x) + ((a)-1)) & ~((a)-1))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

/* -------------- Block header -------------- */
typedef struct block_header {
    size_t size;                  /* size of user payload (bytes) */
    int free;                     /* 1 if free, 0 if used */
    struct block_header *next;    /* next block in heap */
    struct block_header *prev;    /* previous block in heap */
    uint64_t magic;               /* sanity check */
} block_header;

#define HEADER_SIZE ALIGN_UP(sizeof(block_header), ALIGNMENT)

/* -------------- Globals -------------- */
static block_header *head = NULL;   /* start of heap list */
static block_header *tail = NULL;   /* last block in heap */
static pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

/* -------------- Internal helpers -------------- */

static void lock()   { pthread_mutex_lock(&global_mutex); }
static void unlock() { pthread_mutex_unlock(&global_mutex); }

/* get pointer to user memory from header */
static void *header_to_payload(block_header *h) {
    return (void *)((char *)h + HEADER_SIZE);
}

/* get header pointer from user pointer */
static block_header *payload_to_header(void *ptr) {
    if (!ptr) return NULL;
    return (block_header *)((char *)ptr - HEADER_SIZE);
}

/* find the first free block with enough size (first-fit) */
static block_header *find_free_block(size_t size) {
    block_header *cur = head;
    while (cur) {
        if (cur->free && cur->size >= size) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* request more space from OS using sbrk */
static block_header *request_space(block_header *last, size_t size) {
    void *req = sbrk(HEADER_SIZE + size);
    if (req == (void *)-1) return NULL;

    block_header *block = (block_header *)req;
    block->size = size;
    block->free = 0;
    block->next = NULL;
    block->prev = last;
    block->magic = MAGIC;

    if (last) last->next = block;
    if (!head) head = block;
    tail = block;

    return block;
}

/* split a block into used part and a free remainder if large enough */
static void split_block(block_header *block, size_t size) {
    /* only split if remainder would be large enough for a header + MIN_SPLIT_SIZE */
    if (block->size < size + HEADER_SIZE + MIN_SPLIT_SIZE) return;

    /* location of new header */
    char *new_block_addr = (char *)header_to_payload(block) + size;
    block_header *new_block = (block_header *)new_block_addr;

    new_block->size = block->size - size - HEADER_SIZE;
    new_block->free = 1;
    new_block->magic = MAGIC;

    /* re-link */
    new_block->next = block->next;
    new_block->prev = block;
    if (block->next) block->next->prev = new_block;
    block->next = new_block;

    /* shrink original block */
    block->size = size;

    if (tail == block) tail = new_block;
}

/* coalesce block with adjacent free blocks (next and prev) */
static void coalesce(block_header *block) {
    if (!block) return;

    /* merge with next if free */
    if (block->next && block->next->free) {
        block_header *next = block->next;
        if (next->magic != MAGIC) return; /* corrupt */
        block->size += HEADER_SIZE + next->size;
        block->next = next->next;
        if (next->next) next->next->prev = block;
        if (tail == next) tail = block;
    }

    /* merge with prev if free */
    if (block->prev && block->prev->free) {
        block_header *prev = block->prev;
        if (prev->magic != MAGIC) return;
        prev->size += HEADER_SIZE + block->size;
        prev->next = block->next;
        if (block->next) block->next->prev = prev;
        if (tail == block) tail = prev;
        block = prev;
    }
}

/* -------------- Public API -------------- */

void *my_malloc(size_t size) {
    if (size == 0) return NULL;

    lock();

    /* align requested size */
    size_t aligned_size = ALIGN_UP(size, ALIGNMENT);

    block_header *block = find_free_block(aligned_size);
    if (block) {
        /* found a free block, possibly split it */
        block->free = 0;
        split_block(block, aligned_size);
        unlock();
        return header_to_payload(block);
    }

    /* no suitable block found; request more space */
    block_header *last = tail;
    block = request_space(last, aligned_size);
    if (!block) {
        unlock();
        return NULL;
    }

    /* Note: request_space already set block->free = 0 */
    unlock();
    return header_to_payload(block);
}

void my_free(void *ptr) {
    if (!ptr) return;

    lock();
    block_header *block = payload_to_header(ptr);
    if (block->magic != MAGIC) {
        /* corrupted pointer / double free / invalid pointer — ignore in this simple impl */
        unlock();
        return;
    }

    block->free = 1;
    coalesce(block);
    unlock();
}



/* optional: simple sanity debug printer (not thread-safe) */

void debug_print_heap(void) {
    block_header *cur = head;
    printf("Heap blocks:\n");
    while (cur) {
        printf("  %p: size=%zu free=%d prev=%p next=%p\n",
               (void*)cur, cur->size, cur->free, (void*)cur->prev, (void*)cur->next);
        cur = cur->next;
    }
}


int main()
{
  void* ptr = my_malloc(135150); // Allocate 132 KB
  printf("Allocated memory at: %p\n", ptr);

  my_malloc(134150);
  my_malloc(136150);
  my_malloc(138150);
    my_malloc(140150);

  debug_print_heap();

    return 0;}
