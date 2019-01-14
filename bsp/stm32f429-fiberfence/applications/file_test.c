/* 
 * ????:?????? 
 * ???????????????,?????????????? 
 */ 

#include <rtthread.h> 
#include <dfs_posix.h> /* ??????????,????????? */ 

#define TEST_FN    "/test.dat" 

/* ????????? */ 
static char test_data[120], buffer[120]; 

/* ?????? */ 
void readwrite(const char* filename) 
{ 
    int fd; 
    int index, length; 

    /* ?? & ?? ?? */ 
    fd = open(TEST_FN, O_WRONLY | O_CREAT | O_TRUNC, 0); 
    if (fd < 0) 
    { 
        rt_kprintf("open file for write failed\n"); 
        return; 
    } 

    /* ?????? */ 
    for (index = 0; index < sizeof(test_data); index ++) 
    { 
        test_data[index] = index + 27; 
    } 

    /* ???? */ 
    length = write(fd, test_data, sizeof(test_data)); 
    if (length != sizeof(test_data)) 
    { 
        rt_kprintf("write data failed\n"); 
        close(fd); 
        return; 
    } 

    /* ???? */ 
    close(fd); 

    /* ?????????? */ 
    fd = open(TEST_FN, O_WRONLY | O_CREAT | O_APPEND, 0); 
    if (fd < 0) 
    { 
        rt_kprintf("open file for append write failed\n"); 
        return; 
    } 

    length = write(fd, test_data, sizeof(test_data)); 
    if (length != sizeof(test_data)) 
    { 
        rt_kprintf("append write data failed\n"); 
        close(fd); 
        return; 
    } 
    /* ???? */ 
    close(fd); 

    /* ?????????? */ 
    fd = open(TEST_FN, O_RDONLY, 0); 
    if (fd < 0) 
    { 
        rt_kprintf("check: open file for read failed\n"); 
        return; 
    } 

    /* ????(???????????) */ 
    length = read(fd, buffer, sizeof(buffer)); 
    if (length != sizeof(buffer)) 
    { 
        rt_kprintf("check: read file failed\n"); 
        close(fd); 
        return;
    } 

    /* ???????? */ 
    for (index = 0; index < sizeof(test_data); index ++) 
    { 
        if (test_data[index] != buffer[index]) 
        { 
            rt_kprintf("check: check data failed at %d\n", index); 
            close(fd); 
            return; 
        } 
    } 

    /* ????(???????????) */ 
    length = read(fd, buffer, sizeof(buffer)); 
    if (length != sizeof(buffer)) 
    { 
        rt_kprintf("check: read file failed\n"); 
        close(fd); 
        return;
    }

    /* ???????? */ 
    for (index = 0; index < sizeof(test_data); index ++) 
    { 
        if (test_data[index] != buffer[index]) 
        { 
            rt_kprintf("check: check data failed at %d\n", index); 
            close(fd); 
            return; 
        } 
    } 

    /* ??????,???? */ 
    close(fd); 
    /* ???? */ 
    rt_kprintf("read/write done.\n"); 
}

#ifdef RT_USING_FINSH 
#include <finsh.h> 
/* ?????finsh shell ???? */ 
FINSH_FUNCTION_EXPORT(readwrite, perform file read and write test); 
#endif 
