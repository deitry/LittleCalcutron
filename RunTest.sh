#!/bin/sh

# очищаем консоль, очищаем предыдущую сборку и собираем заново
clear && make -f Makefile clean test

# чтобы не вбивать аргументы каждый раз заново
./Run/LittleCalcutron --result_code=no --report_level=short