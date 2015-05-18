#ifndef STRUCT_H
#define STRUCT_H

class InitInfo
{
public:
    int video_width;
    int video_height;

    void parse(InitInfo&, void* src);
    void serialize(void* dst);

    const static int struct_size = 1024;
};

#endif // STRUCT_H
