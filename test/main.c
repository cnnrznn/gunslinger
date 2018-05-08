#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define SIZE 200000

int main()
{

        char *buf1, *buf2, *buf3;
        unsigned long i;
	    int count = -1;
	    unsigned long *ws;

        buf1 = malloc(SIZE * getpagesize());
        if (!buf1) {
                printf("unable to allocate memory\n");
                return 1;
        }
        buf2 = malloc(SIZE * getpagesize());
        if (!buf2) {
                printf("unable to allocate memory\n");
                return 1;
        }
        buf3 = malloc(SIZE * getpagesize());
        if (!buf3) {
                printf("unable to allocate memory\n");
                return 1;
        }
        printf("allocated memory\n");

        for (i=0; i<SIZE; i++)
                buf1[i*getpagesize()] = 'c';
        for (i=0; i<SIZE; i++)
                buf2[i*getpagesize()] = 'c';
        for (i=0; i<SIZE; i++)
                buf3[i*getpagesize()] = 'c';
        printf("initialized memory\n");

        /**********************************************/

        int fd = open("/dev/gunslinger", O_RDWR);
        int pid = getpid();

        printf("%d\n", pid);
        write(fd, &pid, sizeof(int));

        //for (i=SIZE/10; i < SIZE/5; i++)
        //        buf[i*getpagesize()] = 'd';

        //for (i=0; i<30; i++) {
        //        sleep(2);
        //}

	    read(fd, &count, sizeof(int));
        printf("count ws: %d\n", count);

        ws = malloc(count * sizeof(unsigned long));
        if (!ws) {
                printf("couldn't allocate ws buffer\n");
                goto out;
        }

        read(fd, ws, count);

        for (i=0; i<count; i++)
            printf("%lx ", ws[i]);

	    free(ws);
out:
        close(fd);

        return 0;
}
