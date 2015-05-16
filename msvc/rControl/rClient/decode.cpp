#include "decode.h"

#include <boost/thread.hpp>

namespace rc
{
	Decode::Decode(boost::shared_ptr<DataBuffer> data_buf, AVCodecID codec_id)
    {
		data_buf_ = data_buf;
		codec_id_ = codec_id;

		f = fopen("screen.avi", "rb+");
		if (!f)
		{
			printf("open file failed\n");
			exit(0);
		}
		fcount = 0;
		img_buf = new char[1366 * 768 *5];
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

		if (codec->capabilities&CODEC_CAP_TRUNCATED)
			cctx->flags |= CODEC_FLAG_TRUNCATED;

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
		frame_rgb = av_frame_alloc();
		assert(frame);

		while (1)
		{

			if (!io_get_data(this, av_buf.get(), 1024))
			{
				boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(20));
				continue;
			}

			pkt.size = 1024;
			pkt.data = (uint8_t*)(av_buf.get());

			while (pkt.size > 0)
			{
				if (decode_packet() < 0)
				{
					fprintf(stderr, "Decode has an error\n");
					exit(0);
				}
			}
			
		}

    }

    int Decode::decode_packet()
    {
        int got, len;

        len = avcodec_decode_video2(cctx, frame, &got, &pkt);

        if (len < 0)
        {
            return -1;
        }

        if (got)
        {
			fcount++;
			int w = cctx->width;
			int h = cctx->height;

			avpicture_fill((AVPicture*)frame_rgb, (const uint8_t*)img_buf, PIX_FMT_RGB24, w, h);

			sws_context = sws_getContext(w, h, cctx->pix_fmt, w, h, PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
			sws_scale(sws_context, frame->data, frame->linesize, 0, h, frame_rgb->data, frame_rgb->linesize);

			/*char fname[10];
			sprintf(fname, "%d.img", fcount);
			FILE* fimg = fopen(fname, "wb");
			fwrite(frame_rgb->data[0], 1, w*h * 3, fimg);
			fclose(fimg);*/
        }

		pkt.size -= len;
		pkt.data += len;

        return 0;
    }

    int Decode::io_get_data(void *opaque, char *buf, int buf_size)
    {
		Decode* dec = (Decode*)opaque;
		int count = fread(buf, 1, buf_size, dec->f);
		return count;
        /*
        if (dec->data_buf_->read((char*)buf, buf_size))
            return buf_size;
        else
            return 0;*/
    }

}

