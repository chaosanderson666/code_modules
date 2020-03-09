SET dir=..\cli\* 
AStyle.exe --mode=c --style=kr --max-code-length=80 --indent=spaces=8 --pad-oper --pad-comma --pad-header --align-pointer=name %dir%
del /s /q ..\cli\*.orig
rem --remove-braces
rem --options=file
rem --recursive
