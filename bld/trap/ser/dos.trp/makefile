#pmake: build trap remote os_dos cpu_i86 cpu_386 ser x86
host_cpu = i86
host_os  = dos

target_cpu = x86
target_os  = dos

srv = ser
which = TRAP
remote_link = 1
builtin_trap = 1

extra_objs = serint.obj serdos.obj busywait.obj

!include $(trap_dir)/master.mif
