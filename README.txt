README FILE:

-Linux-stable kernel v4.20.13 used.

-Changes to syscalls.h file included in patch file [patch.diff].
		
-The directory "tests" contains the userspace code for testing the process_ancestors syscall and the Makefile. For Makefile of userspace application: [1] Issue "make all" command. [2] Issue "make transfer" command. [3] Enter "root" as password when prompted (target "transfer" is for transferring .o file to QEMU VM through port 2222).

-Issue command "git am < ../patch.diff" and place patch.diff file in one directory above the linux kernel folder [E.g. if linux kernel is all in a folder "linux-stable" then place patch.diff file in the directory containing "linux-stable" folder].



