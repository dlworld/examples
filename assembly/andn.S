.text                           # section declaration

                                                  # we must export the entry point to the ELF linker or
    .global _start              # loader. They conventionally recognize _start as their
                                                  # entry point. Use ld -e foo to override the default.

_start:

                                # write our string to stdout
          movl    $0x0,%ebx      # won't print
#          movl    $0x1,%ebx      # print
          movl    $0x1,%ecx
          andn    %ebx,%ecx,%eax  # !ebx & ecx -> eax
                                  # eax == 0 -> ZF

          and    $0x1, %eax

c1:          
          cmp    $0x1, %eax
          je      out

p1:          
          movl    $len,%edx           # third argument: message length
          movl    $msg,%ecx           # second argument: pointer to message to write
          movl    $1,%ebx             # first argument: file handle (stdout)
          movl    $4,%eax             # system call number (sys_write)
          int     $0x80               # call kernel

                                # and exit
out:
          movl    $0,%ebx             # first argument: exit code
          movl    $1,%eax             # system call number (sys_exit)
          int     $0x80               # call kernel

.data                           # section declaration

msg:
        .ascii    "Hello, world!\n"   # our dear string
        len = . - msg                 # length of our dear string

