@echo off
color 0e

set encryptcmd=encrypt.exe
if not exist %encryptcmd% (
  echo �ļ� %encryptcmd% ������
  goto ENDOK
)

:INPUTNAME
echo.
echo �������ļ���?
set /p dirname=

if "%dirname%" == ""  (
	echo �ļ�������Ϊ��!
	echo ��������:
	goto INPUTNAME
) else (
	echo ����/�����ļ� %dirname%
)

echo ��������Կ?
set /p number=
if "%number%" == ""  (
	echo δ��⵽��Կ����!
	echo ��ʹ���ļ������������:
        @%encryptcmd%  %dirname% %dirname%
) else (
	echo ��ʹ����Կ %number% ���ļ����м���
        @%encryptcmd%  %dirname% %number% 
)

:ENDOK
pause