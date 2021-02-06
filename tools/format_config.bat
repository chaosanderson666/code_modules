SET dir=^
..\btn_scan\*.c ^
..\btn_scan\*.h ^
..\cJSON\*.c ^
..\cJSON\*.h ^
..\task_event\*.c ^
..\task_event\*.h

AStyle.exe ^
--mode=c ^
--style=kr ^
--max-code-length=200 ^
--indent=spaces=8 ^
--pad-oper ^
--pad-comma ^
--pad-header ^
--align-pointer=name %dir%

del /s /q ^
..\btn_scan\*.orig ^
..\cJSON\*.orig ^
..\task_event\*.orig
