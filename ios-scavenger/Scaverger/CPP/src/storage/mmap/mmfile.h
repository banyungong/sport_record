//
// Created by liruopeng on 2023/7/25.
//

#ifndef MMAP_DEMO_MMFILE_H
#define MMAP_DEMO_MMFILE_H

#include "string"

using namespace std;

class MMFile {
private:
    char *mmap_ptr;
    long long file_size;
    string file_path;
public:
    /**
     * 初始化文件，如果文件不存在，会创建文件,并写入file_size大小的数据
     * @param dir
     * @param filename
     */
    void initFile(string dir, string filename, int file_size);

    /**
     * 文件扩容，为原来的2倍
     * @return
     */
    void dilatation();

    /**
     * 映射数据
     */
    void reflection(bool force);

    /**
     * 写入数据(会覆盖所有)
     * @param str
     */
    void write(string str);

    /**
     *
     * @param str
     */
    void append(string str);

    /**
     * 关闭数据映射
     */
    void close();

    string read();
};


#endif //MMAP_DEMO_MMFILE_H
