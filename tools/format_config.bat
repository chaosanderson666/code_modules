SET dir=..\proto_thread\*.c ..\proto_thread\*.h
AStyle.exe --mode=c --style=kr --max-code-length=100 --indent=spaces=8 --pad-oper --pad-comma --pad-header --align-pointer=name %dir%
del /s /q ..\proto_thread\*.orig
rem --remove-braces
rem --options=file
rem --recursive
