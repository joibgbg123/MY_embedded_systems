cmd_/home/pi/MY_embedded_systems/project4/part3/src/kernel_module/Module.symvers := sed 's/ko$$/o/' /home/pi/MY_embedded_systems/project4/part3/src/kernel_module/modules.order | scripts/mod/modpost -m -a   -o /home/pi/MY_embedded_systems/project4/part3/src/kernel_module/Module.symvers -e -i Module.symvers   -T -