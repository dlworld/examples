#!/usr/bin/env python

import os

ret = os.popen('python count.py 1 1').readlines()
assert(int(ret[0].strip()) == 1)
