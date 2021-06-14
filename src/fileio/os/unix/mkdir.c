#include <sys/stat.h>
#include <sys/types.h>

void fileio_mkdir(char *path)
{
    mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}