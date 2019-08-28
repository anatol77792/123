#!/bin/bash
make clean
make


xterm -e './client 5589 5559' &
xterm -e './client2 6559 5557' &
sleep 3
xterm -e './client 5591 5559' &
xterm -e './client2 6560 5557' &
xterm -e './server 5559 5557' &
