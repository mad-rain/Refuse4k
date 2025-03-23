@echo off
del refuse.ok
wcl exe_strp.c -zq
wcc386 main.c -DDPMI -DRELEASE -s -otmrin -3r -fp3 -zp1 -zq
nasm -fbin stub.s -o stub.bin 
wdis main.obj -a -e -p >main.asm
perl correct.pl
wasm main_cr.asm -zq
alink main_cr.obj -oEXE -o main_cr.bin
exe_strp.exe
copy /b stub.bin + main_cr.raw main_cr.com
upx -q -9 main_cr.com -o refuse.ok
copy /b refuse.ok + add.txt refuse.com