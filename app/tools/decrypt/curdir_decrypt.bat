@echo off
color 0e

set encryptcmd=encrypt.exe
if not exist %encryptcmd% (
  echo 文件 %encryptcmd% 不存在
  goto ENDOK
)

:INPUTNAME
echo.
echo 请输入文件索引名?
set /p dirname=

if "%dirname%" == ""  (
	echo 文件索引名不能为空!
	echo 重新输入:
	goto INPUTNAME
) else (
	echo 将使用文件 %dirname% 做为解密索引
)


echo 请输入秘钥?
set /p number=
if "%number%" == ""  (
	echo 未检测到密钥输入!
	echo 将使用文件名对自身解密:
        FOR /F "eol=;  delims=, " %%i in (%dirname%) do @%encryptcmd% %%i %%i
) else (
	echo 将使用秘钥 %number% 对文件进行解密
        FOR /F "eol=;  delims=, " %%i in (%dirname%) do @%encryptcmd% %%i %number%
)

:ENDOK
pause