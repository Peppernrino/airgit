@echo off
set file=%1client\version.inc
set tmpfile=%1client\version.inc.tmp

call git update-index --assume-unchanged %1client\StringDefs.cpp
call git update-index --assume-unchanged %1client\EN_Example.xml

for /F "tokens=*" %%a in ('git describe --abbrev"="4  --dirty"="-d') do echo #define GIT_TAG "%%a" >> %tmpfile%

for /F "tokens=*" %%a in ('git rev-list HEAD --count') do echo #define GIT_COMMIT_COUNT %%a >> %tmpfile%

for /F "tokens=*" %%a in ('git log -1 --format"="%%at') do echo #define VERSION_DATE %%a >> %tmpfile%

echo #define APPNAME_INC "AirDC++" >> %tmpfile%

fc /b %tmpfile% %file% > nul
if errorlevel 1 goto :versionChanged

echo No Version changes detected, using the old version file
goto :end
	
:versionChanged
	echo Version changes detected updating the version file
	copy /y %tmpfile% %file% > nul
goto :end
	
:end
call git update-index --no-assume-unchanged %1client\StringDefs.cpp
call git update-index --no-assume-unchanged %1client\EN_Example.xml
ECHO Y | DEL %tmpfile%