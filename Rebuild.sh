#!/bin/sh

# очищаем консоль, очищаем предыдущую сборку и собираем заново
clear && make -f Makefile clean test all