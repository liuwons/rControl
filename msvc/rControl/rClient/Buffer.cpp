#include "Buffer.h"

namespace rc
{
    DataBuffer::DataBuffer(int buf_size)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);

        buf_size_ = buf_size;
        data = boost::shared_array<char>(new char[buf_size]);
        assert(data.get());
        read_pos_ = 0;
        write_pos_ = 0;
        filled_ = 0;
    }

    bool DataBuffer::read(char* dst, int sz)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);

        if (filled_ < sz)
            return false;

        if (read_pos_ + sz <= buf_size_)
        {
            memcpy(dst, data.get() + read_pos_, sz);
            filled_ -= sz;
            read_pos_ += sz;
        }
        else
        {
            int read_len1 = buf_size_ - read_pos_;
            memcpy(dst, data.get()+read_pos_, read_len1);
            memcpy(dst + read_len1, data.get(), sz - read_len1);

            filled_ -= sz;
            read_pos_ += sz;
            read_pos_ %= buf_size_;
        }
        return true;
    }

    bool DataBuffer::write(char* src, int sz)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);

        if (filled_ + sz > buf_size_)
            return false;

        if (write_pos_ + sz <= buf_size_)
        {
            memcpy(data.get() + write_pos_, src, sz);
            filled_ += sz;
            write_pos_ += sz;
        }
        else
        {
            int write_len1 = buf_size_ - write_pos_;
            memcpy(data.get() + write_pos_, src, write_len1);
            memcpy(data.get(), src + write_len1, sz - write_len1);

            filled_ += sz;
            write_pos_ += sz;
            write_pos_ %= buf_size_;
        }
        return true;
    }
}