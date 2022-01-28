#include <linux/kernel.h> 
#include <stdio.h>
#include <sys/syscall.h> 
#include <unistd.h>
#include <linux/socket.h>

#define __NR_sys_intercept_gps 391
//#define __NR_socket            281

int main(int argc, char *argv[])
{
   // long ret = syscall(__NR_sys_intercept_gps, 5, 5,5L);
	//long ret = syscall(281 ,PF_INET6, SOCK_STREAM, IPPROTO_IP);
	long	ret = syscall(281 ,10, 1, 0);
        printf(">>>>> socket system call invoked %ld \n",ret);
	long ret1 = syscall(__NR_sys_intercept_gps,10, 1, 0);
	printf(" >>>>> socket_system call for GPS invoked %ld \n",ret1);
       //	long ret_gps= syscall(391, 5, 5,5L);
    //    printf("gps system call invoked %ld \n",ret_gps);
   return 0;
}
