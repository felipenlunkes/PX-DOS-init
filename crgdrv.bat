@echo off
echo.
title Compilando Modulo de Carregamento de Drivers do Kernel do PX-DOS
cls
echo.
echo.
echo Compilando o Modulo de Carregamento de Drivers do Kernel do PX-DOS
echo.
echo.
echo O arquivo gerado tem o nome de CRGDRV.SIS
echo.
echo CaReGador de DRiVers do PX-DOS
echo.
echo.
wasmr -zq -zcm -Dmemodel=large drvasm.asm
tcc -O -c -ml crgdrv.c
tlink -x crgdrv+drvasm,crgdrv.sis,,C:\Dev\DOS\pxdosc.lib

if exist CRGDRV.SIS ( GOTO SUCESSO1 ) else ( GOTO ERRO1 )

:SUCESSO1

title Sucesso!
echo.
echo Sucesso ao compilar.
del *.obj
goto SAIR

:ERROR1

title Erro!
echo.
echo Erro ao compilar.
del *.obj
goto SAIR

:SAIR

exit
