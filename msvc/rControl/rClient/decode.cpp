#include "decode.h"

namespace rc
{
    Decode::Decode(boost::shared_ptr<DataBuffer> data_buf)
    {

    }

    bool Decode::init()
    {

    }

    void Decode::start()
    {
        AVPacket pck;
        av_init_packet(&pck);
        pck.data = NULL;
        pck.size = 0;

        while (1)
        {
            int err = av_read_frame(fctx, &pck);
            if (err < 0)
            {
                break;
            }

            int ret;
            AVPacket orig_pck = pck;

            do
            {
                ret = decode_packet(&pck, this);
                if (ret < 0)
                    break;
                pck.data += ret;
                pck.size -= ret;
            } while (pck.size > 0);

            av_free_packet(&orig_pck);
        }
    }

    int Decode::decode_packet(AVPacket* pck, Decode* dec)
    {
        int got;
        int ret;

        if (pck->stream_index == dec->video_id)
        {
            AVFrame* fr = av_frame_alloc();
            ret = avcodec_decode_video2(dec->cctx, fr, &got, pck);

            if (ret < 0)
            {
                av_frame_unref(fr);
                av_frame_free(&fr);
                printf("\ndecode has an error:%d\n", ret);
                return -1;
            }

            if (got)
            {
                sws_scale(dec->sctx, fr->data, fr->linesize, 0, dec->height, dec->pic->data, dec->pic->linesize);
                //acq->mVideoBuffer->AddFrame(acq->img, acq->mFrame);
            }

            av_frame_unref(fr);
            av_frame_free(&fr);

            return ret;
        }
        else
        {
            return pck->size;
        }
    }

    int Decode::io_get_data(void *opaque, uint8_t *buf, int buf_size)
    {
        Decode* dec = (Decode*)opaque;
        if (dec->data_buf_->read((char*)buf, buf_size))
            return buf_size;
        else
            return 0;
    }


    bool Decode::av_init()
    {
        av_register_all();
        fctx = avformat_alloc_context();

        av_buf = av_malloc(AV_IO_BUF_LEN);
        avio = avio_alloc_context((unsigned char*)av_buf, AV_IO_BUF_LEN, 0, this, io_get_data, NULL, NULL);
        fctx->pb = avio;

        return true;
    }

    bool Decode::av_decode_header()
    {
        int err;

        err = avformat_open_input(&fctx, NULL, NULL, NULL);

        if (err != 0)
        {
            printf("open input failed:%d\n", err);
            return false;
        }

        av_input_opened = true;

        err = avformat_find_stream_info(fctx, NULL);

        printf("\n\nfind stream info:%d, streams:%d\n\n", err, fctx->nb_streams);

        if (err != 0)
        {
            return false;
        }

        video_id = -1;
        for (int i = 0; i < fctx->nb_streams; i++)    //区分视频流和音频流
        {
            if (fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) //找到视频流，这里也可以换成音频
            {
                video_id = i;
                break;
            }
        }

        printf("video_id:%d\n", video_id);

        codec = avcodec_find_decoder(fctx->streams[video_id]->codec->codec_id);
        cctx = fctx->streams[video_id]->codec;
        err = avcodec_open2(cctx, codec, NULL);    // 打开解码器

        printf("open codec:%d\n", err);

        if (err != 0)
        {
            return false;
        }

        width = cctx->width;
        height = cctx->height;

        printf("width:%d, height:%d\n", width, height);

        AVRational frame_rate = fctx->streams[video_id]->r_frame_rate;

        pic = new AVPicture;
        avpicture_alloc(pic, PIX_FMT_RGB24, width, height);

        sctx = sws_getContext(width, height, cctx->pix_fmt, width, height, (PixelFormat)PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);

        return true;
    }



}

