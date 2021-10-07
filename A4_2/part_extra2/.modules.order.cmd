cmd_/home/pi/projects/A4_2/part_extra2/modules.order := {   echo /home/pi/projects/A4_2/part_extra2/mydev.ko; :; } | awk '!x[$$0]++' - > /home/pi/projects/A4_2/part_extra2/modules.order
