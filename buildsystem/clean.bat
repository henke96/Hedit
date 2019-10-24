@echo off
if exist bin\ (
    rmdir /S /Q bin
)
if exist build\ (
    rmdir /S /Q build
)