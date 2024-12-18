#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/slab.h>

#define DEVICE_NAME "sound_trigger"

static char *sound_file_path="/tmp/sound.wav";
module_param(sound_file_path,charp, 0664);
MODULE_PARM_DESC(sound_file_path,"Path to the sound file");

void play_sound(void){
    char *argv[]={"/usr/bin/aplay",sound_file_path,NULL};
    char *envp[]={NULL};
    int ret;
    
    pr_info("Triggering sound playback from %s...\n",sound_file_path);
    ret=call_usermodehelper(argv[0],argv,envp,UMH_WAIT_PROC);
    if(ret==0){
        pr_info("Sound played successfully.\n");
    }else{
        pr_err("Failed to play sound.\n");
    }


}

static int sound_device_open(struct inode *inode,struct file *file){
    pr_info("Sound device opened.\n");
    return 0;
}

static int sound_device_release(struct inode *inode,struct file *file){
    pr_info("Sound device closed.\n");
    return 0;
}

static ssize_t sound_device_write(struct file *file,const char __user *buf,size_t count,loff_t *offset){
    char command[128];
    if(copy_from_user(command,buf,count)){
        return -EFAULT;        
    }

    if(strncmp(command,"play_sound",10)==0){
        play_sound();
    }else{
        pr_err("Unknown command: %s\n",command);
    }

    return count;
}

static const struct file_operations sound_fops={
    .owner=THIS_MODULE,
    .open=sound_device_open,
    .release=sound_device_release,
    .write=sound_device_write,
};

static struct miscdevice sound_device={
    .minor=MISC_DYNAMIC_MINOR,
    .name=DEVICE_NAME,
    .fops=&sound_fops,
};

static int __init sound_driver_init(void){
    int error;
    
    error=misc_register(&sound_device);
    if(error){
        pr_err("Failed to register sound device.\n");
        return error;
    }

    pr_info("Sound driver loaded, device created at /dev/%s\n",DEVICE_NAME);
    pr_info("Sound file path: %s\n",sound_file_path);
    return 0;
}

static void __exit sound_driver_exit(void){
    misc_deregister(&sound_device);
    pr_info("Sound driver unloaded.\n");
}

module_init(sound_driver_init);
module_exit(sound_driver_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Katherine");
MODULE_DESCRIPTION("A kernel module to trigger sound playback.");
MODULE_VERSION("v1");
