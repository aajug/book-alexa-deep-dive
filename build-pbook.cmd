@echo off
cd /d %~dp0

set BOOK=alexa-deep-dive.pbook.pdf

if exist %BOOK% del %BOOK%

:: build
cd /d %~dp0review-starter
for /f %%A in ('wsl wslpath -a '%CD%'') do set WSLCD=%%A
set BASHCD=%WSLCD:/mnt/=/%
docker run --rm -v %BASHCD%:/work kauplan/review2.5 /bin/bash -c "export STARTER_TARGET=pbook; cd /work; chmod +x ./make-pbook.sh; ./make-pbook.sh; gem install combine_pdf; rake pdf; rake pdf:nombre"

move book.pdf %~dp0%BOOK%

:: open
start %~dp0%BOOK%
