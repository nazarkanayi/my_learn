#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <ctime>
#include "shared_memory.h"
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <cctype>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>

std::vector<std::string> extractStrings(const char *buffer, size_t bufferSize)
{
    std::vector<std::string> strings;
    std::string currentString;

    for (size_t i = 0; i < bufferSize; ++i)
    {
        if (std::isprint(buffer[i]))
        {
            // Append printable characters to the current string
            currentString += buffer[i];
        }
        else if (!currentString.empty())
        {
            // If we encounter a non-printable character and the current string is not empty,
            // add it to the list of strings and reset the current string
            strings.push_back(currentString);
            currentString.clear();
        }
    }

    // Add the last string if the buffer ends with a printable character
    if (!currentString.empty())
    {
        strings.push_back(currentString);
    }

    return strings;
}

int main()
{
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 0);
    
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    // Immediately unlink to prevent lingering semaphore
    // if (sem_unlink(SEM_NAME) == -1) {
    //     perror("sem_unlink failed");
    //     sem_close(sem);
    //     exit(EXIT_FAILURE);
    // }


    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, TOTAL_MEMORY) == -1)
    {
        perror("ftruncate");
        return 1;
    }

    void *shm = mmap(0, TOTAL_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    // SharedMemory* shm = new (shm_fresh) SharedMemory();

    if (((std::uint64_t)shm & 0x7) != 0U)
    {
        std::cout << "Memory alignment error \n";
    }

    std::atomic<std::uint32_t> *read_index;
    std::atomic<std::uint32_t> *write_index;
    std::uint8_t *buffer;

    read_index = reinterpret_cast<std::atomic<std::uint32_t> *>(shm);

    write_index = reinterpret_cast<std::atomic<std::uint32_t> *>((uint8_t *)shm + 8U);

    buffer = (uint8_t *)shm + 16U;

    SharedMemory circular_buffer = SharedMemory(buffer, read_index, write_index);
    std::cout << "Server is waiting for messages...\n";

    // circular_buffer.Write((uint8_t *)"nazar", 6);

    // circular_buffer.Write((uint8_t *)"nazar", 6);

    // circular_buffer.Write((uint8_t *)"nazar", 6);

    // circular_buffer.Write((uint8_t *)"nazar", 6);

    // circular_buffer.Write((uint8_t *)"exit", 5);

    while (true)
    {

        // wait for semaphore
        if (sem_wait(sem) == -1) {
            perror("sem_wait failed");
            exit(EXIT_FAILURE);
        }
        bool Exit{false};

        char message[BUFFER_SIZE];
        auto ret = circular_buffer.Read((uint8_t *)message, BUFFER_SIZE);
        std::cout << "read start " << ret.first << " " << ret.second << std::endl;

        if (ret.first)
        {
            std::vector<std::string> strings = extractStrings(message, ret.second);

            for (const std::string &str : strings)
            {
                std::cout << str << std::endl;
                if (strcmp(str.c_str(), "exit") == 0)
                {
                    Exit = true;
                    break;
                }
            }
        }

        if (Exit)
        {

            break;
        }

        ///std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    munmap(shm, sizeof(SharedMemory));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_close(sem);

    std::cout << "Server shutting down.\n";
    return 0;
}
