#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys

matrix = [[1, 2, 3], [4,5,6], [7, 8, 9]]

def is_x_in_matrix(x, start_m, start_n, end_m, end_n, last_m, last_n):
    if start_m < 0 or start_n <0 or end_m < 0 or end_n < 0:
        print "%s in not matrix" %str(x)
        return

    if x == matrix[start_m][start_n] or \
        x == matrix[end_m][end_n]:
        print "%s in matrix" %str(x)
        return

    if x > matrix[start_m][start_n]:
        last_m = start_m
        last_n = start_n
        if x < matrix[end_m][end_n]:
            start_m += (end_m - start_m)/2
            start_n += (end_n - start_n)/2
            is_x_in_matrix(x, start_m, start_n, end_m, end_n,
                    last_m, last_n)
        elif x > matrix[end_m][end_n]:
            print x, matrix[end_m][end_n]
            print "test"

    elif x < matrix[start_m][start_n]:
        end_m = start_m
        end_n = start_n
        start_m = (end_m - last_m)/2
        start_n = (end_n - last_n)/2
        last_m = end_m
        last_n = end_n
        is_x_in_matrix(x, start_m, start_n, end_m, end_n, last_m, last_n)


if __name__ == "__main__":
    is_x_in_matrix(int(sys.argv[1]), 0, 0, 2, 2, 0, 0)
