cd %WATCOM%
if exist \tmp.bat del \tmp.bat
sweep -a newer r:\lang\%%f %%l copy r:\lang\%%f %%f >>\tmp.bat
if exist \tmp.bat call \tmp.bat
if exist \tmp.bat del \tmp.bat
