@echo off
setlocal
set o=%CD%
cd %LOCALAPPDATA%\GitHub\PortableGit*\bin
set g=%CD%
set PATH=%PATH%;%g%;
cd %o%
cmd
