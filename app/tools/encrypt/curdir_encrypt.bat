@echo off
color 0e

set encryptcmd= encrypt.exe
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
	echo �ļ����������浽 %dirname%
)

dir /A:-D /B > %dirname%

echo ��������Կ?
set /p number=
if "%number%" == ""  (
	echo δ��⵽��Կ����!
	echo ��ʹ���ļ������������:
        FOR /F "eol=;  delims=, " %%i in (%dirname%) do @%encryptcmd% %%i %%i
) else (
	echo ��ʹ����Կ %number% ���ļ����м���
        FOR /F "eol=;  delims=, " %%i in (%dirname%) do @%encryptcmd% %%i %number%
)


:ENDOK
pause