#!/usr/bin/python

import os

def parse_and_apply_patch(spec_file, patch_dir):
    with open(spec_file) as f:
        for line in f.readlines():
            if line.startswith('BuildRequires'):
                return

            if line.startswith('Patch'):
                patch = line.split(':')[-1].strip()
                cmd = "git am " + os.path.join(patch_dir, patch)
                os.system(cmd)


if __name__ == '__main__':
    parse_and_apply_patch('/home/dlw/rpmbuild/SPECS/qemu-kvm.spec', '/home/dlw/rpmbuild/SOURCES')
