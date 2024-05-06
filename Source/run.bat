@echo off
rem Compilação do arquivo main.c usando SDCC

rem Compilação usando SDCC
sdcc -c buttons.c
sdcc -c DS1307.c
sdcc -c I2C.c
sdcc -c LCD.c
sdcc -c time.c
sdcc -c main.c
sdcc main.rel lcd.rel time.rel i2c.rel buttons.rel DS1307.rel
packihx main.ihx > main.hex

rem Verifica se a compilação foi bem-sucedida
if %errorlevel% equ 0 (
    echo Compilacao bem-sucedida! Pressione qualquer tecla para sair.
    pause >nul
) else (
    echo Erro durante a compilacao. Pressione qualquer tecla para sair.
    pause >nul
)