#include <asm/unistd.h>
#include <linux/linkage.h>
#include <linux/kernel.h>  /* Needed for KERN_ALERT */


asmlinkage long sys_intercept_gps(unsigned int fd, unsigned int cmd,
                                unsigned long arg){
	printk("call intercepted for GPS");
	//call to ioctl 
	return 0;
}
