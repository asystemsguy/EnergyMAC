#include <asm/unistd.h>
#include <linux/linkage.h>
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/syscalls.h> 
#include <linux/socket.h>
#include <linux/cred.h>
//#include <sys/syscall.h>

#define socket_util_id 20


static int getuid(void)
 {
     return current_uid();
 }


asmlinkage long sys_intercept_gps( int domain,  int type,  int  protocol){
	if(domain==AF_INET || domain == AF_INET6){
		printk(">>>>>>>>>>>>>>>>>call intercepted for internet connection for uid-> %d <<<<",getuid());
	
		//call accounting function
		if(is_energy_use_allowed( getuid(), socket_util_id)){
			printk(">>>>>>>>>>>>>>>>>internet connection allowed for for uid-> %d <<<<",getuid());
			sys_update_credits( getuid(), sys_get_credits(getuid()) - sys_get_cost(socket_util_id));
			return  sys_socket(domain, type , protocol);
		}else{
			printk(">>>>>>>>>>>>>>>>>internet connection NOT allowed for for uid-> %d <<<<",getuid()); 
			return -1;
		}
	}
	// allow if it is local call
	return	sys_socket(domain, type , protocol);
}


