for /f "usebackq tokens=*" %%i in (`cd`) do set WORKSPACE=%%i
set WORKSPACE_UNIX=/%WORKSPACE:~0,1%/%WORKSPACE:~3%
set WORKSPACE_UNIX=%WORKSPACE_UNIX:\=/%
echo %WORKSPACE_UNIX%