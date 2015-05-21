#include "Buffer.h"
#include "struct.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

#include "controlpanel.h"

namespace rc
{
    class Decode
    {
    public:
        const static int AV_IO_BUF_LEN = 1 * 1024 * 1024;

        Decode(boost::shared_ptr<DataBuffer> data_buf, AVCodecID codec_id = AV_CODEC_ID_MPEG1VIDEO);
        void start();

        static int io_get_data(void *opaque, char *buf, int buf_size);
        bool av_decode_header();

        int decode_packet();

		int fcount;

        void set_panel(ControlPanel* p);

        void init(InitInfo&);
		
    private:
		SwsContext* sws_context;
		AVCodecID codec_id_;
		AVCodec* codec;
		AVCodecContext* cctx;
		AVFrame* frame;
		AVFrame* frame_rgb;
		AVPacket pkt;

		char* img_buf;

        boost::shared_ptr<DataBuffer> data_buf_;

		boost::shared_array<char> av_buf;

        bool av_input_opened;
        int video_id;

        ControlPanel* panel;

        int width;
        int height;

        boost::mutex init_mutex;
        bool inited;

    };
}
