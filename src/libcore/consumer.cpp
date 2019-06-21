#include <boost/lockfree/spsc_queue.hpp> // ring buffer

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

namespace bip = boost::interprocess;
namespace shm
{
    typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> char_alloc;
    typedef bip::basic_string<char, std::char_traits<char>, char_alloc >      shared_string;

    typedef boost::lockfree::spsc_queue<
        shared_string, 
        boost::lockfree::capacity<200> 
    > ring_buffer;
}

#include <iostream>

int main()
{
    // create segment and corresponding allocator
    bip::managed_shared_memory segment(bip::open_or_create, "MySharedMemory", 65536);
    shm::char_alloc char_alloc(segment.get_segment_manager());

    shm::ring_buffer *queue = segment.find_or_construct<shm::ring_buffer>("queue")();

    while (true)
    {
        shm::shared_string v(char_alloc);
        if (queue->pop(v))
            std::cout << "Processed: '" << v << "'\n";
    }
}
