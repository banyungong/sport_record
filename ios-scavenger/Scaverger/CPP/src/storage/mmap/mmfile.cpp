//
// Created by liruopeng on 2023/7/25.
//

#include <sys/stat.h>
#include <fstream>
#include <sys/fcntl.h>
#include "sys/mman.h"
#include "mmfile.h"
#include <unistd.h>
#include <android/log.h>


using namespace std;

void MMFile::initFile(string dir, string filename, int size) {
    file_path = dir + filename;
    FILE *file;
    if ((file = fopen(dir.c_str(), "rb")) == nullptr) {
        mkdir(dir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    } else {
        fclose(file);
    }
    ifstream readFile;
    readFile.open(file_path.c_str(), ios::in | ios::binary);
    readFile.seekg(0, ios::end);
    long long f_size = readFile.tellg();
    readFile.close();
    if (f_size <= 0) {
        //打开文件
        fstream fFile;
        fFile.open(file_path.c_str(), ios::out | ios::binary);
        file_size = size == 0 ? 1024 * 256 : size;
        char *buffer = new char[file_size];
        memset(buffer, 0, file_size);
        fFile.write(buffer, file_size);
        fFile.close();
        delete[] buffer;
        //如果长度为0，写入256kb数据
        fFile.close();
    }
    file_size = f_size;
}

void MMFile::dilatation() {
    //打开文件
    ofstream oFile;
    oFile.open(file_path.c_str(), ios::out | ios::binary);
    //移动到文件末尾
    oFile.seekp(file_size, ios::beg);
    //追加写入file_size大小的数据
    char *buffer = new char[file_size];
    memset(buffer, 0, file_size);
    oFile.write(buffer, file_size);
    oFile.close();
    delete[] buffer;
    file_size += file_size;
}

void MMFile::reflection(bool force) {
    if (mmap_ptr == nullptr || force) {
        int fd = open(file_path.c_str(), O_RDWR);
        if (fd == -1) {
            return;
        }
        mmap_ptr = (char *) mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ::close(fd);
    }
}

void MMFile::write(string str) {
    reflection(false);
    if (mmap_ptr == MAP_FAILED) {
        return;
    }
    unsigned int str_len = str.size();
    while (str_len > file_size) {
        dilatation();
        reflection(true);
    }
    memcpy(mmap_ptr, str.c_str(), str.size());
    str.clear();
}

void MMFile::append(string str) {
    reflection(false);
    string temp = mmap_ptr;
    unsigned int str_len = temp.size() + str.size();
    while (str_len > file_size) {
        dilatation();
        reflection(true);
    }
    memcpy(mmap_ptr + temp.size(), str.c_str(), str.size());
    str.clear();
    temp.clear();
}

void MMFile::close() {
    if (mmap_ptr != MAP_FAILED) {
        munmap(mmap_ptr, file_size);
        mmap_ptr = nullptr;
        file_size = 0;
    }
}

string MMFile::read() {
    return {mmap_ptr};
}
