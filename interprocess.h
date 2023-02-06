//MIT License
//
//Copyright (c) 2023 TzeLun
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//
//Author: Tze Lun, Lok
//Date : February 6, 2023


#ifndef INTERPROCESS_H
#define INTERPROCESS_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

class SharedMemory {
    public:
        SharedMemory(const char* name="shmem", size_t size=0, bool create=true);
        ~SharedMemory();

        void write(std::string msg);
        const char* read();
        size_t get_size();
        const char* get_name();
        int get_fd();
        bool fail();  // call this for proper error handling of shared memory creation
    
    private:
        void* buffer;  // memory address
        size_t size;  // bytes
        const char* name;  // name of shared memory
        int fd;  // file descriptor
        bool unsuccessful = false;  // safety flag to ensure proper clean up of memory
};

// Creates shared memory which is used for both read and write
// Set "create flag" to false to access an existing shared memory
SharedMemory::SharedMemory(const char* name, size_t size, bool create) {
    this->name = name;
    this->size = size;

    // Set up file descriptor for shared memory
    if (create) {
        this->fd = shm_open(this->name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    }
    else {
        this->fd = shm_open(this->name, O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
    }

    if (this->fd == -1) {
         perror("shm_open");
         this->unsuccessful = true;
    }

    // configure the size of the shared memory
     if (ftruncate(this->fd, this->size) == -1) {
        perror("ftruncate");
         this->unsuccessful = true;
    }

    // Creating the memory address for the shared memory
    this->buffer = mmap(0, this->size, PROT_WRITE | PROT_READ, MAP_SHARED, this->fd, 0);

    if (this->buffer == MAP_FAILED) {
        perror("mmap");
        this->unsuccessful = true;
    }

    std::cout << "SUCCESSFULLY SETUP SHARED MEMORY :: <<NAME :: " << this->name << "; SIZE :: " << this->size << ">>\n";
}

SharedMemory::~SharedMemory() {
    shm_unlink(this->name);
}

// Write a value to the shared memory. String type supported at the moment
void SharedMemory::write(std::string msg) {
    strncpy((char *)this->buffer, msg.data(), this->size);
}

// Read from the shared memory
const char* SharedMemory::read() {
    return (char*)this->buffer;
}

size_t SharedMemory::get_size() {
    return this->size;
}

const char* SharedMemory::get_name() {
    return this->name;
}

int SharedMemory::get_fd() {
    return this->fd;
}

bool SharedMemory::fail() {
    return this->unsuccessful;
}

// Shared memory exclusive process controller
namespace PROCESS {
    void wait(SharedMemory* shm, std::string criteria);
}

// Hold the program until the criteria is met
void PROCESS::wait(SharedMemory* shm, std::string criteria) {
    bool loop = true;
    while (loop) {
        if (std::string(shm->read()) == criteria) {
            loop = false;
        }
    }
}

#endif