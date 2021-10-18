#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/gpio.h>                       // for the GPIO functions 
#include <linux/interrupt.h>                  // for the IRQ code
#include <linux/fs.h>
#include <time.h>

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("JHR"); 
MODULE_DESCRIPTION("A Encoder/LED test driver for the RPi"); 
MODULE_VERSION("0.1");

static unsigned int     gpioLED = 17;             // pin 11 (GPIO17) 
static unsigned int     gpioEncoder = 24;          // pin 13 (GPIO27)
static unsigned int     gpioEncoderB = 25; 
static unsigned int     irqNumber;                // share IRQ num within file 
static unsigned int     numberPresses = 0;        // store number of presses 
static bool             ledOn = 0;                // used to invert state of LED
static long int         old_time = 0;
static long int         counter = 0;
static long int         pps = 0;
static struct timespec  time_now;

static int encoder_read(struct file *, char *, size_t, loff_t *);

// prototype for the custom IRQ handler function, function below 
static irq_handler_t  erpi_gpio_irq_handler(unsigned int irq, 
                                            void *dev_id, struct pt_regs *regs);

static struct file_operations fops = 
{
    .read = encoder_read,
};

static int __init erpi_gpio_init(void) 
{
    int result = 0;
    printk(KERN_INFO "ENCODER_TEST: Initializing the ENCODER_TEST LKM\n");

    if (!gpio_is_valid(gpioLED)) 
    {
        printk(KERN_INFO "ENCODER_TEST: invalid GPIO\n");
        return -ENODEV;
    }   

    ledOn = true;

    gpio_request(gpioLED, "sysfs");          // request LED GPIO
    gpio_direction_output(gpioLED, ledOn);   // set in output mode and on 
    // gpio_set_value(gpioLED, ledOn);       // not reqd - see line above
    gpio_export(gpioLED, false);             // appears in /sys/class/gpio
                                             // false prevents in/out change   
    gpio_request(gpioEncoder, "sysfs");       // set up gpioEncoder   
    gpio_direction_input(gpioEncoder);        // set up as input   
    gpio_export(gpioEncoder, false);          // appears in /sys/class/gpio

    printk(KERN_INFO "ENCODER_TEST: encoder value is currently: %d\n", 
           gpio_get_value(gpioEncoder));

    irqNumber = gpio_to_irq(gpioEncoder);     // map GPIO to IRQ number
    printk(KERN_INFO "ENCODER_TEST: encoder mapped to IRQ: %d\n", irqNumber);

    // This next call requests an interrupt line   
    result = request_irq(irqNumber,          // interrupt number requested            
        (irq_handler_t) erpi_gpio_irq_handler,   // handler function            
        IRQF_TRIGGER_RISING,                     // on rising edge (press, not release)            
        "erpi_gpio_handler",                     // used in /proc/interrupts
        NULL);                                   // *dev_id for shared interrupt lines
    printk(KERN_INFO "ENCODER_TEST: IRQ request result is: %d\n", result); 
    return result;
}

static void __exit erpi_gpio_exit(void) 
{   

    gpio_set_value(gpioLED, 0);              // turn the LED off
    gpio_unexport(gpioLED);                  // unexport the LED GPIO   
    free_irq(irqNumber, NULL);               // free the IRQ number, no *dev_id   
    gpio_unexport(gpioEncoder);               // unexport the Button GPIO   
    gpio_free(gpioLED);                      // free the LED GPIO
    gpio_free(gpioEncoder);                   // free the Button GPIO
    printk(KERN_INFO "ENCODER_TEST: Goodbye from the LKM!\n"); 
}

static int encoder_read(struct file *file, char *buf, size_t len, loff_t *ppos) 
{
    char *data = (char *) pps;

    for (size_t i = 0; i < len; i++){
        buf[i] = data[i];
    } 
    return 0;
}

static irq_handler_t erpi_gpio_irq_handler(unsigned int irq, 
                                           void *dev_id, struct pt_regs *regs) 
{   
    int dir = gpio_get_value(gpioEncoderB);
    
    clock_gettime(CLOCK_REALTIME, &time_now);
    long int time_dif = time_now.tv_nsec-old_time;
    if (time_dif > 0){
        pps = (1000*1000*1000)/(time_dif);
    }
    old_time = time_now.tv_nsec;
    if (dir == 1) {
        pps = -pps;
    }
    
    return (irq_handler_t) IRQ_HANDLED;      // announce IRQ handled 
}

module_init(erpi_gpio_init);
module_exit(erpi_gpio_exit);