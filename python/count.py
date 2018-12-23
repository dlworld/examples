#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys

def count_x(N, x):
    assert(N >= 1)

    counts = 0
    for i in range(1, N + 1):
        while (i):
            j = i % 10;
            i /= 10
            if (j == x):
                counts += 1

    return counts

if __name__ == '__main__':
    N = int(sys.argv[1])
    if (N <= 0):
        print "N must greater than or equal to 1"

    x = int(sys.argv[2])
    if (x not in range(0,9)):
        print "x must in range(0,9)"

    print count_x(N, x)

