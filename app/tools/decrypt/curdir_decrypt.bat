@echo off
color 0e

set encryptcmd=encrypt.exe
if not exist %encryptcmd% (
  echo �ļ� %encryptcmd% ������
  goto ENDOK
)

:INPUTNAME
echo.
echo �������ļ�������?
set /p dirname=

if "%dirname%" == ""  (
	echo �ļ�����������Ϊ��!
	echo ��������:
	goto INPUTNAME
) else (
	echo ��ʹ���ļ� %dirname% ��Ϊ��������
)


echo ��������Կ?
set /p number=
if "%number%" == ""  (
	echo δ��⵽��Կ����!
	echo ��ʹ���ļ������������:
        FOR /F "eol=;  delims=, " %%i in (%dirname%) do @%encryptcmd% %%i %%i
) else (
	echo ��ʹ����Կ %number% ���ļ����н���
        FOR /F "eol=;  delims=, " %%i in (%dirname%) do @%encryptcmd% %%i %number%
)

:ENDOK
pause