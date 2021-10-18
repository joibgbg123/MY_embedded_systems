#!/bin/bash

echo 17 > /sys/class/gpio/export
echo 24 > /sys/class/gpio/export
sleep 0.5
echo "out" > /sys/class/gpio/gpio17/direction
echo "in" > /sys/class/gpio/gpio24/direction

COUNTER=0
while [ $COUNTER -lt 10000 ]; do
    cat /sys/class/gpio/gpio24/value > /sys/class/gpio/gpio17/value
done

echo 17 > /sys/class/gpio/unexport
echo 24 > /sys/class/gpio/unexport