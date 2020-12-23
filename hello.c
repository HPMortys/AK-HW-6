#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "linux/ktime.h"
#include "linux/slab.h"

MODULE_AUTHOR("Moroz Denis IV-81");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint amt = 1;
module_param(amt, uint, S_IRUGO);
MODULE_PARM_DESC(counter, "Amount of times hello world message should be printed");

struct list_head {
	struct list_head *next;
	ktime_t time;
};

static struct list_head *head;

static int __init hello_init(void)
{
	struct list_head *tail;
	uint i;
	head = kmalloc(sizeof(struct list_head *), GFP_KERNEL);
	tail = head;

	

	if(amt == 0){
		pr_warn("WARNING The amount is 0");
	}else if (amt >= 5 && amt <= 10){
		pr_warn("WARNING The amount is range 5 and 10");
	}
	BUG_ON(amt > 10);


	for (i = 1; i < amt ; i++) {
		tail->next = kmalloc(sizeof(struct list_head *), GFP_KERNEL);
		if (tail->next == NULL) {
			while (head != NULL) {
				tail = head;
				head = tail->next;
				kfree(tail);
			}
			BUG();
			return -ENOMEM;
		}
		tail = tail->next;
		tail->next = NULL;
		tail->time = ktime_get();
		pr_info("Hello, world!\n");
	}
	

	return 0;

}

static void __exit hello_exit(void)
{
	struct list_head *tail;

	while (head != NULL) {
		tail = head;
		head = tail->next;
		pr_info("Time: %lld\n", tail->time);
		kfree(tail);
	}
}

module_init(hello_init);
module_exit(hello_exit);

