#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 400000

int main()
{

        char *buf;
        int i;

        buf = malloc(SIZE * getpagesize());

        for (i=0; i<SIZE; i++)
                buf[i*getpagesize()] = 'c';

        /**********************************************/

        int fd = open("/dev/gunslinger", O_RDWR);
        int pid = getpid();

        printf("%d\n", pid);
        write(fd, &pid, sizeof(int));


        //for (i=SIZE/10; i < SIZE/5; i++)
        //        buf[i*getpagesize()] = 'd';

        //for (i=0; i<30; i++) {
        //        sleep(2);
                write(fd, &pid, sizeof(int));
        //}

        close(fd);

        return 0;
}
