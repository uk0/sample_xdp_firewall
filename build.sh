#!/bin/bash

clang -O2 -target bpf -I/usr/include/bpf -c xdc.c -o xdp_prog.o

go build main.go

ip link set dev ens35 xdp obj xdp_prog.o sec xdp_firewall
sleep 10m
ip link set dev ens35 xdp off




