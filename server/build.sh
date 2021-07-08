
#!/bin/sh
 
#你需要发布的程序名称
exe="server"
 
#可执行程序所在目录
des=$PWD
 
deplist=$(ldd $exe | awk '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des