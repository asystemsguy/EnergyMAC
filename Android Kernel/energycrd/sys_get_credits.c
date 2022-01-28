#include <asm/unistd.h>
#include <linux/linkage.h>
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/hashtable.h>
#include <linux/slab.h> /* kmalloc */
#include <linux/list.h> /* hlist_node */

struct app_account_node {
     int uid ;
     long  credits;
     struct hlist_node app_account_hash_list ;
};

struct ops_cost_node {
      int util_id;
      int cost;
      struct hlist_node my_hash_list ;
};

DEFINE_HASHTABLE(energy_account, 3);
DEFINE_HASHTABLE(cost_sheet, 3);

asmlinkage long sys_init_credits(void)
{
    printk("energy account is initialized\n");

    hash_init(energy_account);
    hash_init(cost_sheet);
    return 0;
}

asmlinkage long sys_get_credits(int uid) 
{
    struct app_account_node *curr;
    struct hlist_node *temp;
    
    printk("sys_get_credits is called on uid %d\n",uid);
  
    hash_for_each_possible(energy_account,curr,temp,app_account_hash_list,uid)
    {
        printk("curr->uid %d, uid %d\n",curr->uid,uid);
        if(curr->uid == uid)
        {
           printk(KERN_INFO "entrie found uid=%d, credits=%ld\n", curr->uid, curr->credits);
           return curr->credits;
        }
    }  
	//for system app, return long max value
    return 2147483646l;
}
 
asmlinkage long sys_update_credits(int uid, long credits) 
{
    
    struct app_account_node * curr;
    struct hlist_node *temp; 
    
    printk("sys_update_credits is called on UID %d with credits %ld\n",uid,credits);
    
    //search for uid 
    hash_for_each_possible(energy_account,curr,temp,app_account_hash_list,uid)
    {
       printk("curr->uid %d, uid %d\n",curr->uid,uid);
       if(curr->uid == uid)
        {
           //if found update credits
           printk(KERN_INFO "entrie found uid=%d, credits=%ld\n", curr->uid, curr->credits);
           curr->credits = credits;
           return 1;
       }
   }
   
   //else create a new node and add it to the energy account
   struct app_account_node *new_node = kmalloc(sizeof(struct app_account_node),GFP_KERNEL);
   new_node->uid = uid;
   new_node->credits = credits;
   hash_add(energy_account, &(new_node->app_account_hash_list), new_node->uid);
     
   return 0;
}

asmlinkage long sys_remove_app(int uid)
{
    struct app_account_node *curr;
    struct hlist_node *temp;

    printk("sys_get_credits is called on uid %d\n",uid);

    hash_for_each_possible(energy_account,curr,temp,app_account_hash_list,uid)
    {
        printk("curr->uid %d, uid %d\n",curr->uid,uid);
        if(curr->uid == uid)
        {
           printk(KERN_INFO "entrie found uid=%d, credits=%ld\n", curr->uid, curr->credits);
           hash_del(&(curr->app_account_hash_list));
           return 1;
        }
    }
    
    return 0;
}

//internal function for use by cost prediction
asmlinkage long sys_update_cost_sheet(int util_id, int cost)
{
    struct ops_cost_node * curr;
    struct hlist_node *temp;

    printk("sys_update_cost is called on Util_ID %d with cost %d\n",util_id,cost);

    //search for uid 
    hash_for_each_possible(cost_sheet,curr,temp,my_hash_list,util_id)
    {
       printk("curr->util_id %d, util_id %d\n",curr->util_id,util_id);
       if(curr->util_id == util_id)
        {
           //if found update credits
           printk(KERN_INFO "entrie found util_id=%d, cost=%d\n", curr->util_id, curr->cost);
           curr->cost = cost;
           return 1;
       }
   }

   struct ops_cost_node *new_node = kmalloc(sizeof(struct ops_cost_node),GFP_KERNEL);
   new_node->util_id = util_id;
   new_node->cost = cost;
   hash_add(cost_sheet, &(new_node->my_hash_list), new_node->util_id);
  
   return 0;
}

asmlinkage long sys_get_cost(int util_id)
{
    struct ops_cost_node * curr;
    struct hlist_node *temp;

    printk("sys_get_cost is called on Util_ID %d \n",util_id);

    //search for uid 
    hash_for_each_possible(cost_sheet,curr,temp,my_hash_list,util_id)
    {
       printk("curr->util_id %d, util_id %d\n",curr->util_id,util_id);
       if(curr->util_id == util_id)
        {
           //if found update credits
           printk(KERN_INFO "entrie found util_id=%d, cost=%d\n", curr->util_id, curr->cost);
           return curr->cost;
       }
   }
   
   return 0;
}

asmlinkage long is_energy_use_allowed(int uid, int util_id)
{
    long available_credits = sys_get_credits(uid);
    long cost = sys_get_cost(util_id);
    printk("available credits ->> %ld, cost ->> %ld", available_credits, cost);
    if(available_credits >= cost)
    {
        return 1;
    }
    else
    {
        return 0;
    } 
}


