@echo off
call bmode

call docd wclass\pm\obj32
viperb -r wclass.tgt
wmake -i -h -f project.mk

call docd ..\..\win\obj
viperb -r wclass.tgt
wmake -i -h -f project.mk

call docd ..\..\win\nt
viperb -r wclass.tgt
wmake -i -h -f project.mk

call docd ..\..\dos\obj32
viperb -r wclass.tgt
wmake -i -h -f project.mk

call docd ..\..\dos\obj
viperb -r wclass.tgt
wmake -i -h -f project.mk

cd ..\..\..
