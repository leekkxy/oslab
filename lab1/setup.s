!功能为分别显示光标位置、内存容量、磁盘主要参数
entry start
start:

!设置数据段和栈段，栈段为0000：0000~0000：0x7e00
xor ax,ax
mov ss,ax
mov sp,#0x7e00
mov ax,#0x07e0
mov ds,ax

!显示光标信息和光标位置数字
mov bp,#cmsg
call puts
xor bh,bh
mov ah,#3
int 0x10
xor ah,ah
mov al,dh
call dtoa
mov al,dl
call dtoa

!显示内存信息
mov bp,#mmsg
call puts
mov ah,#0x88
int 0x15
call dtoa

!将0x41号中断处的磁盘信息共16B读到内存0x90000处
xor ax,ax
mov ds,ax
lds si,[4*0x41]
mov ax,#0x9000
mov es,ax
xor di,di
mov cx,#16
rep 
movsb

!将ds指向0x9000，以读取磁盘信息
mov ax,es
mov ds,ax

!显示磁盘信息1，即柱面数
mov bp,#dmsg1
call puts


mov ax,[0]
call dtoa

!显示磁盘信息2，即磁头数
mov bp,#dmsg2
call puts

mov al,[2]
xor ah,ah
call dtoa

!显示磁盘信息3，即扇区数
mov bp,#dmsg3
call puts

mov ax,[0x0e]
xor bh,bh
call dtoa

over:jmp over


!将ax中的十进制数字显示在光标位置，使用了0x10号BIOS中断的0E号功能，
!即tty显示模式，自动将光标后移
!输入：ax中存放要输出的数字
dtoa:
push ax
push bx
push cx
push dx

mov bx,#10
xor cx,cx
xor dx,dx

s0:
div bx
mov dh,#0x0e
add dl,#0x30
push dx
inc cx
xor dx,dx
test ax,ax
jnz s0

xor bh,bh
mov bl,#7

s1:
pop ax
int 0x10
sub cx,#1
jnz s1

mov ax,#0x0e0d
int 0x10
mov ax,#0x0e0a
int 0x10

pop dx
pop cx
pop bx
pop ax
ret

!功能为将[0x07e0:bp]处的字符串以tty模式输出
!输入为bp=字符串存放的偏移地址
!借用了BIOS 0x10号中断的0x13号功能
puts:
push ax
push bx
push cx
push dx
push es

xor bh,bh
mov ah,#3
int 0x10

mov ax,#0x07e0
mov es,ax
mov cx,#25
mov bx,#0x0007
mov ax,#0x1301
int 0x10

pop es
pop dx
pop cx
pop bx
pop ax

ret


cmsg:
.byte 13,10
.ascii 'The cursur pos is :'
.byte 13,10,13,10

mmsg:
.byte 13,10
.ascii 'The memory size is:'
.byte 13,10,13,10
dmsg1:
.byte 13,10
.ascii 'The cylindr nbr is:'
.byte 13,10,13,10
dmsg2:
.byte 13,10
.ascii 'The head   nbr is :'
.byte 13,10,13,10
dmsg3:
.byte 13,10
.ascii 'The section nbr is:'
.byte 13,10,13,10
