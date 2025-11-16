// shared_spsc.h
#pragma once

#include <atomic>
#include <cstdint>
#include <cstddef>

#ifndef RING_CAPACITY
#define RING_CAPACITY 1024
#endif

#ifndef SLOT_DATA_BYTES
#define SLOT_DATA_BYTES 1024
#endif

static constexpr size_t RING_SIZE = RING_CAPACITY;
static constexpr size_t SLOT_DATA = SLOT_DATA_BYTES;

struct SlotHeader {
    std::atomic<uint64_t> seq; // Vyukov seq
    uint32_t len;
    uint8_t pad[4];
    uint8_t data[SLOT_DATA];
};

struct SharedMeta {
    std::atomic<uint64_t> head; // consumer ticket
    std::atomic<uint64_t> tail; // producer ticket
    uint64_t capacity;
    uint64_t slot_bytes;
    uint8_t reserved[16];
};

struct SPSCBuffer {
    SharedMeta meta;
    SlotHeader slots[RING_SIZE];
};

