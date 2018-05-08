#!/usr/bin/python3

import os
import sys

pid = os.getpid().to_bytes(4, byteorder=sys.byteorder)
print(pid)

# open device
fd = os.open('/dev/gunslinger', os.O_RDWR)

# input pid
os.write(fd, pid)

# output result
count = int.from_bytes(os.read(fd, 4), byteorder=sys.byteorder)
print('count: {}'.format(count))

ws = os.read(fd, count * 8)
print(ws)

# close device
os.close(fd)
