#include <stdio.h> 
#include <linux/kernel.h> 
#include <sys/syscall.h> 
#include <unistd.h> 
#include <assert.h>

#define __NR_sys_init_credits 384
#define __NR_sys_get_credits 385
#define __NR_sys_update_credits 386
#define __NR_sys_remove_app   387
#define __NR_sys_update_cost_sheet 388
#define __NR_sys_get_cost  389
#define __NR_sys_is_energy_use_allowed 390


void test_credit_update(int user, int credits)
{
   int update_status = syscall(__NR_sys_update_credits,user,credits); //Invoking our own system call
   int updated_credits = syscall(__NR_sys_get_credits,user);
   printf("Update call invoked for user %d  update_status %d , expected_credits %d, actual_credits %d\n",user, update_status, credits, updated_credits);
   assert(updated_credits == credits); //checking for correct credits update
}

void test_cost_update(int util_id, int cost)
{
   int update_status = syscall(__NR_sys_update_cost_sheet,util_id,cost); //Invoking system call to test cost sheet
   int update_cost = syscall(__NR_sys_get_cost,util_id); //Get costs for the same id
   printf("Updated cost invoked for a operation %d, update_status %d, expected_cost %d, actual_cost %d\n",util_id, update_status, cost, update_cost);
   assert(cost == update_cost);
}

int main(int argc, char *argv[]) 
{ 

printf("-------------------TEST 1-----------------------\n");
printf("\n");
printf("Testing inital update for user 1 with credits 20\n");
long init_status = syscall(__NR_sys_init_credits); //Initalizing credit accounting system

test_credit_update(1,20);

printf("Testing update for exitsing user 1 by decreasing credits 10\n");
test_credit_update(1,10);

printf("Testing update for exitsing user 1 by increasing credits 30\n");
test_credit_update(1,30);

printf("Testing update for second user\n");
test_credit_update(2,20);

printf("Testing update for third user\n");
test_credit_update(3,20);

printf("Testing update for second user\n"); 
test_credit_update(2,10);

printf("TEST 1 passed !!!\n");

printf("-------------------TEST 2-------------------------\n");
printf("\n");
printf("Testing reinitalizing sequence\n"); 
init_status = syscall(__NR_sys_init_credits); //Initalizing credit accounting system
int credits = syscall(__NR_sys_get_credits,1); //Reading credits for existing user
printf("updated credits after initialization for user 1 is %d\n",credits);
assert(credits != 30); //30 is latest  updated value for user 1

printf("Testing remove app functionality\n");
printf("Updating credits for app 1\n");
test_credit_update(1,30);

printf("Removing app 1 ");
int status = syscall(__NR_sys_remove_app,1);
printf("with status %d\n", status);

credits = syscall(__NR_sys_get_credits,1); //Reading credits for removed app 1
printf("credits after app 1 is removed is %d\n",credits);
//assert(credits == -1);

printf("TEST 2 passed!!!\n");

printf("-------------------TEST 3--------------------------\n");
printf("\n");
printf("Testing update cost functionality\n");
test_cost_update(1234, 23);

test_cost_update(1235, 34);
test_cost_update(20,10); // this is for socket call

test_credit_update(10063,100);

printf("Updating cost for user 1\n");
test_credit_update(1,30);

printf("Testing is_allowed functionality for true case\n");
int flag = syscall(__NR_sys_is_energy_use_allowed,1,1234);
printf("Testing for user 1 with 30 credits and operation 1234 with cost 24 is_allowed: %d\n",flag);
assert(flag==1);


printf("Testing is allowed functionality for true case\n");
flag = syscall(__NR_sys_is_energy_use_allowed,1,1235);
printf("Testing for user 1 with 30 credits and operation 1235 with cost 34 is_allowed: %d\n",flag);
assert(flag==0);

printf("TEST 3 passed !!!\n");

printf("ALL TESTS PASSED :)\n");

return 0;  
} 
