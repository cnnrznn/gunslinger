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
wsl = []

for i in range(0, len(ws), 8):
    wsl.append(int.from_bytes(ws[i:i+8], byteorder=sys.byteorder))

print(wsl)

# close device
os.close(fd)
