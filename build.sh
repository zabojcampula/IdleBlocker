windres IdleBlocker.rc -o IdleBlocker-resources.o
gcc IdleBlocker.c IdleBlocker-resources.o -o IdleBlocker.exe -mwindows