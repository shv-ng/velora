#!/usr/bin/env bash

echo "Starting test runner..."

gcc ./tmp/generated_runner.c \
    $( fd -t f --glob "*.c" --exclude "main.c" --exclude "generated_runner.c") \
    -o ./tmp/test_suite

./tmp/test_suite


