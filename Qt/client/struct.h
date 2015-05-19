#ifndef STRUCT_H
#define STRUCT_H

class InitInfo
{
public:
    InitInfo();
    bool valid;
    int video_width;
    int video_height;

    void parse(void* src);
    void serialize(void* dst);

    const static int struct_size = 1024;
};

#endif // STRUCT_H
