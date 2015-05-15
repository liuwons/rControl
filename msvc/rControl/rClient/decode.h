#include "Buffer.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

namespace rc
{
    class Decode
    {
    public:
        const static int AV_IO_BUF_LEN = 1 * 1024 * 1024;

        Decode(boost::shared_ptr<DataBuffer> data_buf);
        bool init();
        bool av_init();
        void start();

        static int io_get_data(void *opaque, uint8_t *buf, int buf_size);
        bool av_decode_header();

        static int decode_packet(AVPacket* pck, Decode* dec);

    private:
        boost::shared_ptr<DataBuffer> data_buf_;

        AVCodec* codec;
        AVCodecContext* cctx;
        AVFormatContext* fctx;
        SwsContext      *sctx;
        AVIOContext * avio;
        AVPicture* pic;
        void* av_buf;

        bool av_input_opened;
        int video_id;
        int width, height;
    };
}