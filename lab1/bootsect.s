!功能为显示一段字符串，并且读取软盘的2，3号两个扇区
!即将setup读入0x0000:0x7e00处，最后跳转到setup

entry s
s:

!读取光标位置
mov ah,#3
xor bh,bh
int 0x10

!显示字符串
mov ax,#0x07c0
mov es,ax
mov bp,#msg
mov cx,#25
mov ax,#0x1301
mov bx,#0x0007
int 0x10

!读取0号软盘的0柱面0磁道2号和3号扇区
!没有读取错误控制，应该有的
mov ax,#0x0202
mov cx,#0x0002
xor dx,dx
mov bx,#0x0200
int 0x13

jmpi 0,0x07e0

msg:
.byte 13,10
.ascii 'Now in Bootsect  !'
.byte 13,10,13,10

.org 510
.word 0xaa55
