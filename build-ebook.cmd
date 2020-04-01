@echo off
cd /d %~dp0

set BOOK=alexa-deep-dive.ebook.pdf

if exist %BOOK% del %BOOK%

:: build
cd /d %~dp0review-starter
for /f %%A in ('wsl wslpath -a '%CD%'') do set WSLCD=%%A
set BASHCD=%WSLCD:/mnt/=/%
docker run --rm -v %BASHCD%:/work kauplan/review2.5 /bin/bash -c "export STARTER_TARGET=ebook; cd /work; rake pdf"

move book.pdf %~dp0%BOOK%

:: open
start %~dp0%BOOK%
