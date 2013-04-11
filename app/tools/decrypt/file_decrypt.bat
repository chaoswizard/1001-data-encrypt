@echo off
color 0e

set encryptcmd=encrypt.exe
if not exist %encryptcmd% (
  echo 文件 %encryptcmd% 不存在
  goto ENDOK
)

:INPUTNAME
echo.
echo 请输入文件名?
set /p dirname=

if "%dirname%" == ""  (
	echo 文件名不能为空!
	echo 重新输入:
	goto INPUTNAME
) else (
	echo 加密/解密文件 %dirname%
)

echo 请输入秘钥?
set /p number=
if "%number%" == ""  (
	echo 未检测到密钥输入!
	echo 将使用文件名对自身加密:
        @%encryptcmd%  %dirname% %dirname%
) else (
	echo 将使用秘钥 %number% 对文件进行加密
        @%encryptcmd%  %dirname% %number% 
)

:ENDOK
pause