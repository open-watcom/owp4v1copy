NAME = fgrep
OBJS =  $(name).obj

!include $(posix_dir)/mif/makeone.mif

fgrep.obj: ../egrep/egrep.c $(posix_autodepends)
        $(cc) $[@ $(cflags) $(extra_cflags) -fo=$@ -DFGREP