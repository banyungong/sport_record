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
        f_size = size == 0 ? 1024 * 256 : size;
        char *buffer = new char[f_size];
        memset(buffer, 0, f_size);
        fFile.write(buffer, f_size);
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
    oFile.open(file_path.c_str(),  ios::binary | ios::app);
    //移动到文件末尾
//    oFile.seekp(file_size, ios::end);
    //追加写入file_size大小的数据
    char *buffer = new char[file_size];
    memset(buffer, 0, file_size);
    oFile.write(buffer, file_size);
    oFile.close();
    delete[] buffer;
    file_size += file_size;
}

void MMFile::reflection(bool force) {
    if (force && mmap_ptr != nullptr) {
        munmap(mmap_ptr, file_size);
        mmap_ptr = nullptr;
    }
    if (mmap_ptr == nullptr) {
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
    unsigned int str_len = 0;
    unsigned int offset = 0;
    if (mmap_ptr != MAP_FAILED) {
        offset = ::strlen(mmap_ptr);
    }
    str_len = offset + str.size();
    while (str_len > file_size) {
        dilatation();
        reflection(true);
    }
    memcpy(mmap_ptr + offset, str.c_str(), str.size());
    str.clear();
}

void MMFile::close(bool remove) {
    if (mmap_ptr != MAP_FAILED) {
        munmap(mmap_ptr, file_size);
    }
    mmap_ptr = nullptr;
    file_size = 0;
    if (remove && !file_path.empty()) {
        ::remove(file_path.c_str());
    }
}

string MMFile::read() {
    reflection(false);
    if (mmap_ptr == nullptr || mmap_ptr == MAP_FAILED) {
        return "";
    }

    return {mmap_ptr};
}

