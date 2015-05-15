#ifndef RC_BUFFER_H
#define RC_BUFFER_H

#include <boost/smart_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include <queue>

namespace rc
{
    class Frame
    {
        
    };

    class DataBuffer
    {
    public:
        const static int DEFAULT_BUFFER_SIZE = 1024 * 1024;

        DataBuffer(int buf_size = DEFAULT_BUFFER_SIZE);

        bool read(char* dst, int sz);
        bool write(char* src, int sz);

    private:
        int buf_size_;
        int read_pos_;
        int write_pos_;
        int filled_;
        boost::shared_array<char> data;

        boost::mutex mutex_;
    };

}

#endif