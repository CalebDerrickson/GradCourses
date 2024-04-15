@ECHO OFF
REM Clean everything

ECHO "Cleaning everything..."

REM Testbed
make -f "Makefile.testbed.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)


ECHO "All assemblies cleaned successfully."