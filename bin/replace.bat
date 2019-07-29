@echo off
copy /y D:\TuYuErp\build-TuYuErp-Desktop_Qt_5_8_0_MSVC2015_32bit-Release\release\TuYuErp.exe D:\TuYuErp\TuYuErp\bin
set exs=*.ini
cd "D:\TuYuErp\build-TuYuErp-Desktop_Qt_5_8_0_MSVC2015_32bit-Release\MysqlTableConfig"
dir
for /r %%i in (%exs%) do (
  echo %%i
  copy /y %%i D:\TuYuErp\TuYuErp\bin\MysqlTableConfig
  )
 
pause