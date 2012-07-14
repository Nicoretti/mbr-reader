/**
 * Copyright (C) 2012, Nicola Coretti
 *
 * Author: Nicola Coretti
 * Version: 0.1.0
 * Contact: nico.coretti@googlemail.com
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
using namespace std;

void PrintMBR(uint8_t* mbr, uint16_t mbr_size) {

    for (uint16_t index = 0; index < mbr_size; index++) {
        if (((index % 32) == 0) && (index != 0)) {
           cout << endl; 
        }
        cout << setw(2) << setfill('0') << hex << (int) mbr[index] << " ";
    }
    cout << endl;
}

void PrintMagicNumberOfMBR(uint8_t* mbr, uint16_t mbr_size) {

    cout << endl;
    cout << "Magic Number of MBR: " << hex << setw(2) << setfill('0');
    cout << (int) mbr[mbr_size -2] << " " << setw(2) << setfill('0');
    cout << (int) mbr[mbr_size -1] << endl;
}

// buffer size has to be a multible of 2
uint8_t* ConvertBufferToBigEndian(uint8_t* buffer, uint32_t buffer_size) {

    if ((buffer_size % 2) != 0) {

        throw "Buffer size has to be a multiple of 2";
    }

    // switch bytes
    for (uint32_t index = 0; index < buffer_size; index += 2) {

        buffer[index] ^= buffer[index + 1];
        buffer[index + 1] ^= buffer[index];
        buffer[index] ^= buffer[index + 1];
    }

    return buffer;
}

int main(int argc, char* argv[], char* envp[]) {

    if  (argc == 2) {

        // create string which contains the path to the device (hdd)
        const char* dev_dir = "/dev/";
        uint16_t device_name_length = strlen(argv[1]);
        uint16_t buffer_size = device_name_length + strlen(dev_dir) + 1;
        char* path = new char[buffer_size];
        path[0] = '\0';
        path = strcat(path, dev_dir);
        path = strcat(path, argv[1]);
 
        int hdd_fd = open(path, O_RDONLY);
        // error occured?
        if (hdd_fd == -1) {
            cerr << strerror(errno);
            exit(EXIT_FAILURE);
        }
        
        // read mbr => the first 512 Bytes
        uint16_t mbr_size = 512;
        uint8_t* mbr = new uint8_t[mbr_size];
        ssize_t bytes_read = read(hdd_fd, mbr, mbr_size); 
        if (bytes_read == -1 ) {
            cerr << strerror(errno);
            exit(EXIT_FAILURE);
        } 
        ConvertBufferToBigEndian(mbr, mbr_size);
        PrintMBR(mbr, mbr_size);
        PrintMagicNumberOfMBR(mbr, mbr_size);
    }
    else {
        
        cerr << "No hdd name supplied." << endl;
        exit(EXIT_FAILURE);
    }

}
