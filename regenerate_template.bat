@echo off

echo Cleaning up
del AOC\AOC_Day*.*

FOR /L %%A IN (1,1,25) DO (
  call :GenDay %%A
)

EXIT /B 0

:GenDay

echo Generating Day %1

type nul >> AOC\AOC_Day%1Part1.txt
type nul >> AOC\AOC_Day%1Part2.txt
type nul >> AOC\AOC_Day%1Test.txt

powershell -Command "(gc AOC\AOC_Template.cpp) -replace 'DayX', 'Day%1' | Out-File -encoding ASCII AOC\AOC_Day%1.cpp"

EXIT /B 0
