ECHO OFF

IF [%1] == [] ECHO "No operation specified!" & EXIT 1

REM install chocolatey
@"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"

REM figure out dependencies
SET "chocodeps= "
SET "needdoxygen=0"
IF "%1" == "docs" SET "needdoxygen=1"
IF "%1" == "pack" SET "needdoxygen=1"
IF %needdoxygen% EQU 1 SET "chocodeps=doxygen.install graphviz"

SET "needmingw=0"
IF "%1" == "pack" SET "needmingw=1"
IF "%1" == "test" SET "needmingw=1"
IF %needmingw% EQU 1 SET "chocodeps=%chocodeps% mingw qt6-base-dev"

REM install dependencies
IF "%chocodeps%" == "" (
	ECHO "No packages specified to install."
) ELSE (
	choco install %chocodeps%
)

SET "SCRIPT_DIR=%~dp0"
SET "REPO=%SCRIPT_DIR%.."
SET CONFIGURATION_OPTIONS=-G MinGW Makefiles -DCALGO_QT=NO

cmake -P "%SCRIPT_DIR%cmake\%1.cmake"
