@echo off

set index=%1

if "%index%"=="" set index=1

FOR /L %%A IN (%index%,1,25) DO (
  call :GenDay %%A
)

EXIT /B 0

:GenDay

echo Generating Day %1

del AOC\AOC_Day%1Part1.txt
del AOC\AOC_Day%1Part2.txt
del AOC\AOC_Day%1Test.txt
del AOC\AOC_Day%1.cpp

type nul >> AOC\AOC_Day%1Part1.txt
type nul >> AOC\AOC_Day%1Part2.txt
type nul >> AOC\AOC_Day%1Test.txt

powershell -Command "(gc AOC\AOC_Template.cpp) -replace 'DayX', 'Day%1' | Out-File -encoding ASCII AOC\AOC_Day%1.cpp"

EXIT /B 0
