#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>

void error_and_die(const char*) __attribute__((noreturn));

void error_and_die(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void)
{
    int r;
    const char* memname = "sample";
    const size_t region_size = (size_t)sysconf(_SC_PAGE_SIZE);

        fprintf(stdout, "Space available for [unsigned] chars: %lu\nSpace available for [unsigned] short ints: %lu\nSpace available for [unsigned] ints: %lu\nSpace available for [unsigned] long ints: %lu\nSpace available for [unsigned] long long ints: %lu\n", region_size/sizeof(char), region_size/sizeof(short int), region_size/sizeof(int), region_size/sizeof(long int), region_size/sizeof(long long int));

        int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
        if(fd == -1){
            error_and_die("shm_open");
        }

        /* The truncate() and ftruncate() functions cause the regular file named by path or referenced by fd to be truncated to a size of precisely length bytes.
        * If the file previously was larger than this size, the extra data is lost.
        * If the file previously was shorter, it is extended, and the extended part reads as null bytes ('\0').
        * The file offset is not changed.
        * If the size changed, then the st_ctime and st_mtime fields (respectively, time of last status change and time of last modification; see stat(2)) for the file are updated, and the set-user-ID and set-group-ID mode bits may be cleared.
        * With ftruncate(), the file must be open for writing; with truncate(), the file must be writable.
        * On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
        */
        r = ftruncate(fd, (long)region_size);
        if(r != 0){
            error_and_die("ftruncate");
        }

        /*
        * mmap()  creates a new mapping in the virtual address space of the calling process.
        * The starting address for the new mapping is specified in addr.
        * The length argument specifies the length of the mapping.
        * If addr is NULL, then the kernel chooses the address at which to create the mapping; this is the most portable method of creating a new mapping.
        * If addr is not NULL, then the kernel takes it as a hint about where to place the mapping; on Linux, the mapping will be created at a nearby page boundary.
        * The address of the new mapping is returned as the result of the call.
        * The contents of a file mapping (as opposed to an anonymous mapping; see MAP_ANONYMOUS below), are initialized using length bytes starting at offset offset in the file (or other object) referred to by the file descriptor fd.
        * offset must be a multiple of the page size as returned by sysconf(_SC_PAGE_SIZE).
        * The prot argument describes the desired memory protection of the mapping (and must not conflict with the open mode of the file).
        * It is either PROT_NONE or the bitwise OR of one or more of the following flags:
        * PROT_EXEC Pages may be executed.
        * PROT_READ Pages may be read.
        * PROT_WRITE Pages may be written.
        * PROT_NONE Pages may not be accessed.
        * The flags argument determines whether updates to the mapping are visible to other processes mapping the same region, and whether updates are carried through to the underlying file.
        * This behavior is determined by including exactly one of the following values in flags:
        * MAP_SHARED Share this mapping.
        * Updates to the mapping are visible to other processes that map this file, and are carried through to the underlying file.
        * (To precisely control when updates are carried through to the underlying file requires the use of msync(2).)
        * MAP_PRIVATE Create a private copy-on-write mapping.
        * Updates to the mapping are not visible to other processes mapping the same file, and are not carried through to the underlying file.
        * It is unspecified whether changes made to the file after the mmap() call are visible in the mapped region.
        * On success, mmap() returns a pointer to the mapped area.  On error, the value MAP_FAILED (that is, (void *) -1) is returned, and errno is set to indicate the cause of the error.
        */
        void* ptr = mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(ptr == MAP_FAILED){
            error_and_die("mmap");
        }

        /* close() closes a file descriptor, so that it no longer refers to any file and may be reused.
        */
        // Note: We can close the file descriptor because after mmap() we now have a pointer to the shared memory region.
        close(fd);

        pid_t pid = fork();
        if(pid == 0){
            u_long* d = (u_long*)ptr;
            unsigned long int* e = (unsigned long int*)(ptr+8);
            *d = 0xdbeebee;
            *e = 0xf3f3fff;
            exit(0);
        }else{
            int status;
            waitpid(pid, &status, 0);
            printf("child wrote %#lx\n", *(u_long*)ptr);
            printf("child wrote %#lx\n", *(unsigned long int*)(ptr+8));
        }


        r = munmap(ptr, region_size);
        if(r != 0){
            error_and_die("munmap");
        }

        r = shm_unlink(memname);
        if(r != 0){
            error_and_die("shm_unlink");
        }

    return 0;
}
