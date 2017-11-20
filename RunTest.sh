#!/bin/sh

# очищаем консоль, очищаем предыдущую сборку и собираем заново
./Rebuild.sh

# чтобы не вбивать аргументы каждый раз заново
./Run/LittleCalcutronTest --result_code=no --report_level=short