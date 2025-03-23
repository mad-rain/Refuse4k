nasm -fobj timer.s -o timer.obj
wcc386 main.c -DDPMI -s -otmir -fp5 -5r -w4 -zq
wlink system pmodew file {timer.obj main.obj} name refuse
upx -9 -q refuse.exe
