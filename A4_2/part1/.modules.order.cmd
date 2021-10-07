cmd_/home/pi/projects/A4_2/part1/modules.order := {   echo /home/pi/projects/A4_2/part1/hello.ko; :; } | awk '!x[$$0]++' - > /home/pi/projects/A4_2/part1/modules.order
