cshell: cshell.o  parg.o chcwd.o cumask.o cshell.h ls rm rmdir touch mkdir chmod mv cp ln ping
	gcc cshell.o  parg.o chcwd.o cumask.o cshell.h -o cshell

cumask.o: cumask.c
	gcc -c cumask.c
cshell.o: cshell.c
	gcc -c cshell.c
ls: ls.c cshell.h
	gcc ls.c cshell.h -o ls
parg.o: parg.c
	gcc -c parg.c
rm: rm.c
	gcc rm.c -o rm
rmdir: rmdir.c
	gcc rmdir.c -o rmdir
touch: touch.c cumask.c cshell.h
	gcc touch.c cumask.c cshell.h -o touch
mkdir: mkdir.c cumask.c cshell.h
	gcc mkdir.c cumask.c cshell.h -o mkdir
chmod: chmod.c
	gcc chmod.c -o chmod
mv: mv.c
	gcc mv.c -o mv
cp: cp.c cumask.o cshell.h
	gcc cp.c cumask.o cshell.h -o cp
chcwd.o: chcwd.c
	gcc -c chcwd.c 
ln: ln.c
	gcc ln.c -o ln
ping: ping.c
	gcc ping.c -o ping
