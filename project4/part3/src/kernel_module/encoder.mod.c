#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x306ff876, "module_layout" },
	{ 0xfe990052, "gpio_free" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x76575e7a, "gpiod_unexport" },
	{ 0xb14ac629, "gpiod_set_raw_value" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x7067949f, "gpiod_to_irq" },
	{ 0x25ea8b4b, "gpiod_direction_input" },
	{ 0x1624448b, "gpiod_export" },
	{ 0x95a7762c, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x3e67c189, "__register_chrdev" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xdff5b32b, "gpiod_get_raw_value" },
	{ 0xf6ca1147, "gpio_to_desc" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xc5850110, "printk" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F02B740734B432F843A2551");
