SET dir=..\task_event\*.c ..\task_event\*.h
AStyle.exe --mode=c --style=kr --max-code-length=100 --indent=spaces=8 --pad-oper --pad-comma --pad-header --align-pointer=name %dir%
del /s /q ..\task_event\*.orig
rem --remove-braces
rem --options=file
rem --recursive
