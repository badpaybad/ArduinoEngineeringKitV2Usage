// Example of configuring a Linux serial port for raw access.
// Written by Tyler Montbriand, 2007.  Freeware.

// See	http://man-wiki.net/index.php/3:termios
//	http://man-wiki.net/index.php/4:tty_ioctl
#include "tcgets.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdio.h>
#include <ctype.h>

#include <stdio.h>

// // For a standalone test, do:
// // gcc -DMAIN_TEST tcgets.c -o tcgets
// #ifdef MAIN_TEST
// int main(int argc, char *argv[])
// {
// 	const char *settings="19200N81";
// 	const char *term=DEFAULT_TERM;
// 	int fd;

// 	if(argc >= 2)
// 		term=argv[1];

// 	if(argc >= 3)
// 		settings=argv[2];

// 	fd=termios_open(term, settings);

// 	if(fd<0)
// 	{
// 		fprintf(stderr, "Couldn't open %s\n", term);
// 		return(1);
// 	}

// 	fprintf(stderr, "Opened %s as %s\n", term, settings);

// 	{
// 		char in;
// 		fprintf(stderr, "Attempting blocking read timeout...\n");
// 		fprintf(stderr, "Returned: %d\n", read(fd, &in, 1));
// 	}

// 	close(fd);

// 	return(0);
// }
// #endif

struct valid_pairs
{
	int out;
	int in;
};

/// static const struct valid_pairs valid_parity[], valid_stopbits[], valid_bits[], valid_baud[];

static const struct valid_pairs valid_parity[] = {
	{0, 'N'},				// No parity
	{PARENB, 'E'},			// Even parity
	{PARENB | PARODD, 'O'}, // Odd parity
	{0, -1}};

static const struct valid_pairs valid_stopbits[] = {
	{0, 1},
	{CSTOPB, 2},
	{0, -1}};

static const struct valid_pairs valid_bits[] = {
	{CS5, 5},
	{CS6, 6},
	{CS7, 7},
	{CS8, 8},
	{0, -1}};

static const struct valid_pairs valid_baud[] = {
	{B0, 0},
	{B50, 50},
	{B75, 75},
	{B110, 110},
	{B134, 134},
	{B150, 150},
	{B200, 200},
	{B300, 300},
	{B600, 600},
	{B1200, 1200},
	{B1800, 1800},
	{B2400, 2400},
	{B4800, 4800},
	{B9600, 9600},
	{B19200, 19200},
	{B38400, 38400},
	{B57600, 57600},
	{B115200, 115200},
	{B230400, 230400},
	{0, -1}};

static struct termios cur_termios; // Current terminal settings

int termios_open(const char *fname, const char *cmdstr)
{
	int fd = open(fname, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd < 0)
		goto TERMIOS_OPEN_ERR;

	if (ttyname(fd) == NULL)
		goto TERMIOS_OPEN_ERR;

	if (termios_set(fd, cmdstr) < 0)
		goto TERMIOS_OPEN_ERR;

	return (fd);

TERMIOS_OPEN_ERR:
	if (fd >= 0)
		close(fd);

	return (-1);
}

int isvalid(unsigned int in, const struct valid_pairs valid[])
{
	int n;
	for (n = 0; valid[n].in >= 0; n++)
	{

		if (valid[n].in == in)
		{
			fprintf(stderr, "%d]\t%d\t<=>\t%d\n", n, valid[n].in, in);
			return (valid[n].out);
		}
	}

	return (-1);
}

int termios_set(int fd, const char *cmdstr)
{

	int baud = -1, bits = -1, stopbits = -1;
	unsigned char parity = 0;
	int values;

	if (cmdstr == NULL)
		cmdstr = DEFAULT_TTY_SETTINGS;

	values = sscanf(cmdstr, "%i%[nNeEoO]%01i%01i",
					&baud, &parity, &bits, &stopbits);
	if (values != 4)
	{
		fprintf(stderr, "Bad spec string '%s'\n", cmdstr);
		return (-1);
	}

	baud = isvalid(baud, valid_baud);

	if (baud < 0)
	{
		fprintf(stderr, "Bad spec string '%s':  Invalid baud rate\n",
				cmdstr);
		return (-1);
	}

	parity = isvalid((char)toupper(parity), valid_parity);
	if (parity == (unsigned char)(-1))
	{
		fprintf(stderr, "Bad spec string '%s':  "
						"Invalid parity specifier\n",
				cmdstr);
		return (-1);
	}

	bits = isvalid(bits, valid_bits);
	if (bits < 0)
	{
		fprintf(stderr, "Bad spec string '%s':  "
						"Invalid bits specifier\n",
				cmdstr);
		return (-1);
	}

	stopbits = isvalid(stopbits, valid_stopbits);
	if (stopbits < 0)
	{
		fprintf(stderr, "Bad spec string %s:  "
						"Invalid stop bits specifier\n",
				cmdstr);
		return (-1);
	}

	fprintf(stderr, "Configuring for %s\n", cmdstr);

	// Set baud rate
	cfsetispeed(&cur_termios, baud);
	cfsetospeed(&cur_termios, baud);

	// Ignore errors, BREAK condition; restart on any character
	cur_termios.c_iflag = IGNBRK | IGNPAR | IXANY;
	cur_termios.c_oflag = 0;
	cur_termios.c_lflag = 0;

	// Enable reading, ignore modem lines, enable CTS/RTS
	cur_termios.c_cflag = CLOCAL | CREAD | CRTSCTS;

	cur_termios.c_cflag |= stopbits;
	cur_termios.c_cflag |= bits;
	cur_termios.c_cflag |= parity;

	// Enable error checking when parity enabled
	if (parity != 0)
		cur_termios.c_iflag &= ~(IGNPAR | IGNBRK);

	cur_termios.c_cc[VMIN] = 0;					   // Timeout needs 0 chars
	cur_termios.c_cc[VTIME] = DEFAULT_TTY_TIMEOUT; // Timeout in tenths

	return (tcsetattr(fd, TCSADRAIN, &cur_termios));
}

int termios_defaults(int fd)
{
	return (termios_set(fd, NULL));
}

void dunp_test()
{
	printf("dunp_test: PING -> PONG\r\n");
	fflush(stdout);
}

char* dunp_test_stdString(char *text)
{

	printf("dunp_test_stdString: %s\r\n", text);
	fflush(stdout);

	size_t sizeA = strlen(text);
	char *b = " PONG ";
	size_t sizeB = strlen(b);
	size_t size = sizeof(char) * (sizeA + sizeB + 1); // Size of our new memory block large enough to contain both a and b.
													  // Leave additional space for null terminator
	char *c =(char*) malloc(size);

	memcpy(c, text, sizeA);		 // Copy a into the first half of c
	memcpy(c + sizeA, b, sizeB); // Copy b into the second half
	c[sizeA + sizeB] = '\0';	 // Assign null terminator to last character

	return c;
}

int _dunp_state = -1;
const char *_dunp_options = "";

char _dunp_buf[256];

int _dunp_lock=0;

int dunp_open(const char *fname, const char *cmdstr)
{
	_dunp_options = cmdstr;
	int r = termios_open(fname, cmdstr);
	_dunp_state = 1;
	return r;
}

int dunp_write(int fd, char *text)
{
	while (_dunp_lock==1)
	{
		usleep(100);
	}
	
	_dunp_lock=1;

	int n = write(fd, text, 6);
	// usleep((7 + 25) * 100);
	tcdrain(fd);

	_dunp_lock=0;
	return n;
}

void dunp_lock(){

	_dunp_lock=1;
}

void dunp_lock_release(){

	_dunp_lock=0;
}

char *dunp_read(int fd)
{
	while (_dunp_lock==1)
	{
		usleep(100);
	}
	_dunp_lock=1;
	
	int n = read(fd, _dunp_buf, 255);
	char *r;
	if (n > 0)
	{
		_dunp_buf[n] = 0;

		r = &_dunp_buf[0];
	}
	_dunp_lock=0;
	return r;
}

int dunp_close(int fd)
{
	close(fd);
	_dunp_state = 0;
	return -1;
}
