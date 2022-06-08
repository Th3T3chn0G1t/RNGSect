#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>

#define SECTOR 512

bool check_offset(const uint8_t* const restrict buff, const size_t index, bool* const restrict out_hlt, size_t* const restrict out_stride) {
    if(buff[index] == 0b11110100) {
        *out_hlt = true;
        *out_stride = 1;
        return true;
    }
    *out_hlt = false;
    switch(buff[index] >> 3) {
        case 0b00000:
            if(   (buff[index] & 0b100) == 0b100   &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b100   &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b00001:
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b00010:
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b00011:
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b00100: 
            if(   (buff[index] & 0b111) == 0b111                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b00101:
            if(   (buff[index] & 0b111) == 0b111                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            break;
        case 0b00110:
            if(   (buff[index] & 0b111) == 0b111                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b00111:
            if(   (buff[index] & 0b111) == 0b111                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b100) == 0b000   &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 3; return true;}
            break;
        case 0b01000: *out_stride = 1; return true;
        case 0b01001: *out_stride = 1; return true;
        case 0b10000:
            if(  ((buff[index] & 0b110) == 0b100)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b111) == 0b000)  && ((buff[index + 1] >> 3) == 0b11001   )) {*out_stride = 3; return true;}
            if(  ((buff[index] & 0b111) == 0b000)  && ((buff[index + 1] >> 3) == 0b11100   )) {*out_stride = 3; return true;}
            if(  ((buff[index] & 0b111) == 0b000)  && ((buff[index + 1] >> 3) == 0b11110   )) {*out_stride = 3; return true;}
            if((!((buff[index] & 0b111) == 0b001)) && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 3; return true;}
            if((!((buff[index] & 0b111) == 0b001)) && ((buff[index + 1] >> 3) == 0b11010   )) {*out_stride = 3; return true;}
            if((!((buff[index] & 0b111) == 0b001)) && ((buff[index + 1] >> 3) == 0b11011   )) {*out_stride = 3; return true;}
            if((!((buff[index] & 0b111) == 0b001)) && ((buff[index + 1] >> 3) == 0b11101   )) {*out_stride = 3; return true;}
            if((!((buff[index] & 0b111) == 0b001)) && ((buff[index + 1] >> 3) == 0b11111   )) {*out_stride = 3; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 4; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11001   )) {*out_stride = 4; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11011   )) {*out_stride = 4; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11100   )) {*out_stride = 4; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11101   )) {*out_stride = 4; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11110   )) {*out_stride = 4; return true;}
            if(  ((buff[index] & 0b111) == 0b001)  && ((buff[index + 1] >> 3) == 0b11111   )) {*out_stride = 4; return true;}
            break;
        case 0b10001: *out_stride = 2; return ((buff[index] & 0b100) == 0b000) && (buff[index + 1] & 0b11000000);
        case 0b10010: *out_stride = 1; return true;
        case 0b10011:
            if(   (buff[index] & 0b111) == 0b000                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b001                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b011                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b111                                            ) {*out_stride = 1; return true;}
            break;
        case 0b10101:
            if(   (buff[index] & 0b111) == 0b000                                            ) {*out_stride = 2; return true;}
            if(   (buff[index] & 0b111) == 0b001                                            ) {*out_stride = 3; return true;}
            break;
        case 0b10110: *out_stride = 2; return true;
        case 0b10111: *out_stride = 3; return true;
        case 0b11000:
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 3; return true;}
            if(  ((buff[index] & 0b111) == 0b110)  && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 3; return true;}
            if(  ((buff[index] & 0b111) == 0b111)  && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 4; return true;}
            break;
        case 0b11010:
            if(  ((buff[index] & 0b100) == 0b000)  &&  (buff[index + 1]       &  0b11000000)) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b111) == 0b100)  &&  (buff[index + 1]       == 0b00001010)) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b111) == 0b101)  &&  (buff[index + 1]       == 0b00001010)) {*out_stride = 2; return true;}
            break;
        case 0b11110:
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 1; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11001   )) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11010   )) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11011   )) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11100   )) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 3; return true;}
            if(  ((buff[index] & 0b110) == 0b111)  && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 4; return true;}
            break;
        case 0b11111:
            if(   (buff[index] & 0b111) == 0b001                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b010                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b011                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b100                                            ) {*out_stride = 1; return true;}
            if(   (buff[index] & 0b111) == 0b101                                            ) {*out_stride = 1; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11000   )) {*out_stride = 2; return true;}
            if(  ((buff[index] & 0b110) == 0b110)  && ((buff[index + 1] >> 3) == 0b11001   )) {*out_stride = 2; return true;}
            break;
    }

    return false;
}

bool check_sector(const uint8_t* const restrict buff) {
    if(buff[SECTOR - 2] != 0x55 || buff[SECTOR - 1] != 0xAA) return false;

    bool is_halt = false;
    size_t stride = 1;
    for(size_t i = 0; !is_halt && i < SECTOR - 2; i += stride) {
        bool valid = check_offset(buff, i, &is_halt, &stride);
        if(!valid) return false;
    }
    return true;
}

int main(void) {
    unlink("os_sector.bin");

    uint8_t buff[SECTOR] = {0};

    int rand = open("/dev/random", O_RDONLY);
    bool is_os = false;
    while(!is_os) {
        read(rand, buff, SECTOR);
        is_os = check_sector(buff);
    }
    close(rand);

    int out = open("os_sector.bin", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    write(out, buff, SECTOR);
    close(out);
}
