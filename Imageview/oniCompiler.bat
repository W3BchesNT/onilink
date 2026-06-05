@echo off
cls
echo ==========================================
echo       COMPILING FASM PROJECT...
echo ==========================================

cd /d "%~dp0"
set INCLUDE=%USERPROFILE%\Desktop\FASM\INCLUDE

:: Удаляем старые ссылки, чтобы не засорять папку
if exist app_dev_*.exe del /f /q app_dev_*.exe

:: Чистая компиляция в твой стандартный main.exe
"%USERPROFILE%\Desktop\FASM\fasm.exe" main.asm main.exe

if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Compiled successfully!
    echo ==========================================
    
    :: Генерируем уникальное имя для обхода кэша Windows
    set "DEV_EXE=app_dev_%RANDOM%.exe"
    
    :: Создаем жесткую ссылку (Hard Link) — Windows увидит её как новый четкий файл
    mklink /H "%DEV_EXE%" main.exe >nul
    
    echo Running clear app: %DEV_EXE%
    
    :: Запускаем напрямую в фоновом режиме и мгновенно тушим консоль
    setlocal
    "%DEV_EXE%"
    exit
) else (
    echo.
    echo [ERROR] Compilation failed! Check errors above.
    echo ==========================================
    pause
)
