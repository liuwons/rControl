#include "Buffer.h"

namespace rc
{
	Frame::Frame()
	{
	}

	void Frame::init(int w, int h, int dl)
	{
		data = boost::shared_array<char>(new char[dl]);
		assert(data.get());
		width = w;
		height = h;
	}

	Frame::~Frame()
	{

	}

	FrameBuffer::FrameBuffer(int w, int h, int dl, int sz)
	{
		frames = boost::shared_array<Frame>(new Frame[sz]);
		for (int i = 0; i < sz; i++)
		{
			frames[i].init(w, h, dl);
		}
		frame_size_ = sz;
		read_pos_ = 0;
		write_pos_ = 0;
		filled_ = 0;
	}

	FrameBuffer::FrameBuffer(Frame& f, int sz)
	{
		FrameBuffer(f.width, f.height, f.data_len, sz);
	}

	bool FrameBuffer::read_frame(Frame& dst)
	{
        boost::lock_guard<boost::mutex> lock(mutex_);

		if (!filled_)
			return false;

		dst.data_len = frames[read_pos_].data_len;
		dst.width = frames[read_pos_].width;
		dst.height = frames[read_pos_].height;
		memcpy(dst.data.get(), frames[read_pos_].data.get(), dst.data_len);

		read_pos_++;
		read_pos_ %= frame_size_;
		filled_--;

		return true;

	}

	bool FrameBuffer::write_frame(Frame& src)
	{
		boost::lock_guard<boost::mutex> lock(mutex_);

		if (filled_ == frame_size_)
			return false;

		frames[write_pos_].data_len = src.data_len;
		frames[write_pos_].width = src.width;
		frames[write_pos_].height = src.height;
		memcpy(frames[write_pos_].data.get(), src.data.get(), src.data_len);

		write_pos_++;
		write_pos_ %= frame_size_;
		filled_++;

		return true;
	}

    DataBuffer::DataBuffer(int buf_size)
    {
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
        {
            return false;
        }

        if (read_pos_ + sz <= buf_size_)
        {
            memcpy(dst, data.get() + read_pos_, sz);
        }
        else
        {
            int read_len1 = buf_size_ - read_pos_;
            memcpy(dst, data.get()+read_pos_, read_len1);
            memcpy(dst + read_len1, data.get(), sz - read_len1);
        }

		filled_ -= sz;
		read_pos_ += sz;
		read_pos_ %= buf_size_;

        return true;
    }

    bool DataBuffer::write(char* src, int sz)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);

        if (filled_ + sz > buf_size_)
        {
            printf("write failed\n");
            return false;
        }

        if (write_pos_ + sz <= buf_size_)
        {
            memcpy(data.get() + write_pos_, src, sz);
        }
        else
        {
            int write_len1 = buf_size_ - write_pos_;
            memcpy(data.get() + write_pos_, src, write_len1);
            memcpy(data.get(), src + write_len1, sz - write_len1);
        }

		filled_ += sz;
		write_pos_ += sz;
		write_pos_ %= buf_size_;

        return true;
    }
}
