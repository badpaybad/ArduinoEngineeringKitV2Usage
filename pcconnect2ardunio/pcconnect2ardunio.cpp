#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include "libs/tcgets.h"
#include "libs/tcgets.c"

int main(int argc, char *argv[])
{
    /// https://www.unix.com/programming/204475-error-resource-temporarily-unavailable-serial-writing-termios-library.html
    // const char *settings="19200N81";
    // const char *term=DEFAULT_TERM;
     const char *settings = "115200N81";
    //const char *settings = "9600N81";
    const char *term = "/dev/ttyACM0";
    int fd;
    char buf[256];
    int n;

    if (argc >= 2)
        term = argv[1];

    if (argc >= 3)
        settings = argv[2];

    // if (fd < 0)
    // {
    //     fprintf(stderr, "Couldn't open %s\n", term);
    //     return (1);
    // }

    // fprintf(stderr, "Opened %s as %s\n", term, settings);
    fd = termios_open(term, settings);
    while (1)
    {
        char in;
        // fprintf(stderr, "Attempting blocking read timeout...\n");
        // fprintf(stderr, "Returned: %d\n", read(fd, &in, 255));

        n = write(fd, "Hello!", 6);
        // usleep((7 + 25) * 100);
        tcdrain(fd);

        // fd = termios_open(term, settings);
        n = read(fd, buf, 255);
        if (n > 0)
        {
            buf[n] = 0;
            printf("read %i bytes: %s", n, buf);
        }
        // 1000000 = 1s
        usleep(100000);
        
    }
    close(fd);

    return (0);
}

// int main(int argc, char **argv)
// {
// // // https://developer.apple.com/forums/thread/697472
//     int fd;
//     char *portname = "/dev/ttyACM0";
//     char buf[256];
//     int n;
//     int i;
//     int count = 0;
//     int baudrate = B115200;
//     struct termios toptions;

//     if (fd > 0)
//     {
//         close(fd);
//     }

//     fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);
//     if (fd == -1)
//     {
//         perror("serialport_init: Unable to open port ");
//         return -1;
//     }

//     if (tcgetattr(fd, &toptions) < 0)
//     {
//         perror("serialport_init: Couldn't get term attributes");
//         return -1;
//     }
//     speed_t brate = baudrate; // let you override switch below if needed
//     switch (baudrate)
//     {
//     case 4800:
//         brate = B4800;
//         break;
//     case 9600:
//         brate = B9600;
//         break;
// #if defined B14400
//     case 14400:
//         brate = B14400;
//         break;
// #endif
//     case 19200:
//         brate = B19200;
//         break;
// #if defined B28800
//     case 28800:
//         brate = B28800;
//         break;
// #endif
//     case 38400:
//         brate = B38400;
//         break;
//     case 57600:
//         brate = B57600;
//         break;
//     case 115200:
//         brate = B115200;
//         break;
//     }
//     cfsetispeed(&toptions, brate);
//     cfsetospeed(&toptions, brate);

//     // 8N1
//     toptions.c_cflag &= ~PARENB;
//     toptions.c_cflag &= ~CSTOPB;
//     toptions.c_cflag &= ~CSIZE;
//     toptions.c_cflag |= CS8;
//     // no flow control
//     toptions.c_cflag &= ~CRTSCTS;

//     toptions.c_cflag |= CREAD | CLOCAL;          // turn on READ & ignore ctrl lines
//     toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

//     toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
//     toptions.c_oflag &= ~OPOST;                          // make raw

//     toptions.c_cc[VMIN] = 0;
//     toptions.c_cc[VTIME] = 20;

//     if (tcsetattr(fd, TCSANOW, &toptions) < 0)
//     {
//         perror("init_serialport: Couldn't set term attributes");
//         return -1;
//     }

//     while (1)
//     {
//         // n = write(fd, "Hello!", 6);
//         // usleep((7 + 25) * 100);
//         // if (n < 0)
//         // {
//         //     perror("Write failed");
//         // }

//         n = read(fd, buf, 255);
//         if (n > 0)
//         {
//             buf[n] = 0;
//             printf("read %i bytes: %s", n, buf);
//         }

//         // n = write(fd, "Hello!", 6);
//         // if (n < 0)
//         // {
//         //     perror("Write failed");
//         // }

//         if (count == 0)
//         {

//             count++;
//         }
//         usleep(100000);
//     }

//     close(fd);
//     return 0;
// }