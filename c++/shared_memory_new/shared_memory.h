#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <atomic>
#include <cstdint>

#define SHM_NAME "/shm_circular_buffer"
#define BUFFER_SIZE 1024U
#define INDEX_LEN  16U
#define TOTAL_MEMORY INDEX_LEN+BUFFER_SIZE
#define SEM_NAME "my_example_sem"

class SharedMemory
{

public:
    SharedMemory() = default;

    SharedMemory(std::uint8_t *buffer, std::atomic<std::uint32_t> *read_index, std::atomic<std::uint32_t> *write_index)
        : buffer_m{buffer}, read_index_m{read_index}, write_index_m{write_index}
    {
        LoadReadIndex();
        LoadWriteIndex();


    }

    SharedMemory(const SharedMemory &other) = default;

    SharedMemory(SharedMemory &&other) = default;

    SharedMemory &operator=(const SharedMemory &other) = default;

    SharedMemory &operator=(SharedMemory &&other) = default;

    std::uint32_t GetCapacity()
    {
        return BUFFER_SIZE - 1;
    }

    std::uint32_t GetUsedSpace()
    {
        std::uint32_t size{0u};

        if (current_write_index >= current_read_index)
        {
            size = current_write_index - current_read_index;
        }
        else
        {
            size = (BUFFER_SIZE + current_write_index) - current_read_index;
        }
        return size;
    }

    void LoadReadIndex()
    {
        current_read_index = read_index_m->load(std::memory_order_seq_cst);
    }

    void LoadWriteIndex()
    {
        current_write_index = write_index_m->load(std::memory_order_seq_cst);
        std::cout << "current_write_index " << current_write_index<< std::endl;

    }

    std::uint32_t GetAvailableSpace()
    {
        return GetCapacity() - GetUsedSpace();
    }

    void AdvanceIndex(std::uint32_t* Index, std::uint32_t count)
    {
        *Index = *Index+count;

        if(*Index >= BUFFER_SIZE)
        {
          *Index = (*Index) - BUFFER_SIZE;
        }

        std::cout << "AdvanceIndex" << *Index << " " << count << std::endl;

    }

    void StoreWriteIndex()
    {
         write_index_m->store(current_write_index,std::memory_order_seq_cst);
    }

    void StoreReadIndex()
    {
         read_index_m->store(current_read_index,std::memory_order_seq_cst);
    }


    bool IsEmpty()
    {
        return (current_write_index == current_read_index);
    }


    bool Write(std::uint8_t *buffer, uint16_t len)
    {
        LoadReadIndex();
        LoadWriteIndex();
        if (len > GetAvailableSpace())
        {
            std::cout << "Buffer full" << std::endl;

            return false;
        }

        if(current_write_index+len > BUFFER_SIZE)
         {
            std::cout << "Buffer overwrite" << std::endl;
            std::uint32_t size = BUFFER_SIZE-current_write_index;
            std::memcpy(buffer_m+current_write_index,buffer,size);
            AdvanceIndex(&current_write_index,size);

            std::memcpy(buffer_m+current_write_index,buffer,len - size);
            AdvanceIndex(&current_write_index,len - size);
         }
         else
         {
           std::memcpy(buffer_m+current_write_index,buffer,len);
           AdvanceIndex(&current_write_index,len);
         }
        
        
         

         StoreWriteIndex();

         return true;

    }

    std::pair<bool, std::uint32_t> Read(std::uint8_t *buffer, uint16_t len)
    {
        LoadReadIndex();
        LoadWriteIndex();
       if(IsEmpty())
       {
         std::cout << "Buffer empty" << std::endl;

         return {false,0U};
       }


       auto Datalen = len > GetUsedSpace() ? GetUsedSpace() : len;

       if(Datalen+current_read_index > BUFFER_SIZE)
       {
          std::cout << "Buffer rollover" << std::endl;
          std::uint32_t size = BUFFER_SIZE-current_read_index;
          std::memcpy(buffer,buffer_m+current_read_index,size);

          AdvanceIndex(&current_read_index,size);

          std::memcpy(buffer+size,buffer_m+current_read_index,Datalen-size);

          AdvanceIndex(&current_read_index,Datalen-size);
       }
       else
       {
        std::memcpy(buffer,buffer_m+current_read_index,Datalen);

        AdvanceIndex(&current_read_index,Datalen);
       }

       

        StoreReadIndex();

        return {true,Datalen};

    }

private:
    std::uint8_t *buffer_m{nullptr};
    std::atomic<std::uint32_t> *read_index_m{nullptr};
    std::atomic<std::uint32_t> *write_index_m{nullptr};

    std::uint32_t current_read_index{0U};

    std::uint32_t current_write_index{0U};
};

#endif
