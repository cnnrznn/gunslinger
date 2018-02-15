#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
        int fd = open("/dev/gunslinger", O_RDWR);
        int pid = getpid();

        printf("%d\n", pid);
        write(fd, &pid, sizeof(int));

        close(fd);

        return 0;
}
