#include "decode.h"

#include <boost/thread.hpp>

namespace rc
{
	Decode::Decode(boost::shared_ptr<DataBuffer> data_buf, AVCodecID codec_id)
    {
		data_buf_ = data_buf;
		codec_id_ = codec_id;
    }

    void Decode::start()
    {
		av_register_all();
		printf("av register succeed\n");
		av_init_packet(&pkt);

		av_buf = boost::shared_array<char>(new char[AV_IO_BUF_LEN]);
		assert(av_buf.get());

		codec = avcodec_find_decoder(codec_id_);
		assert(codec);
		printf("av find codec succeed\n");
		
		cctx = avcodec_alloc_context3(codec);
		assert(cctx);

		if (avcodec_open2(cctx, codec, NULL) < 0)
		{
			fprintf(stderr, "Could not open codec\n");
			exit(0);
		}
		else
		{
			printf("av open codec succeed\n");
		}

		frame = av_frame_alloc();
		assert(frame);

		while (1)
		{
			if (!io_get_data(this, av_buf.get(), 1024))
			{
				printf("io get data: 0\n");
				boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(20));
				continue;
			}

			printf("io get data:1024\n");
			pkt.size = 1024;
			pkt.data = (uint8_t*)(av_buf.get());
			if (decode_packet(&pkt, this) < 0)
			{
				fprintf(stderr, "Decode has an error\n");
				exit(0);
			}
		}

    }

    int Decode::decode_packet(AVPacket* pck, Decode* dec)
    {
        int got;
        int len;

        AVFrame* fr = av_frame_alloc();
        len = avcodec_decode_video2(dec->cctx, fr, &got, pck);

        if (len < 0)
        {
            av_frame_unref(fr);
            av_frame_free(&fr);
            printf("\ndecode has an error:%d\n");
            return -1;
        }

        if (got)
        {
			printf("got frame\n");
            //sws_scale(dec->sctx, fr->data, fr->linesize, 0, dec->height, dec->pic->data, dec->pic->linesize);
            //acq->mVideoBuffer->AddFrame(acq->img, acq->mFrame);
        }

		if (pck->data)
		{
			pck->size -= len;
			pck->data += len;
		}

        av_frame_unref(fr);
        av_frame_free(&fr);

        return 0;
    }

    int Decode::io_get_data(void *opaque, char *buf, int buf_size)
    {
        Decode* dec = (Decode*)opaque;
        if (dec->data_buf_->read((char*)buf, buf_size))
            return buf_size;
        else
            return 0;
    }

        //sctx = sws_getContext(width, height, cctx->pix_fmt, width, height, (PixelFormat)PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);


}

