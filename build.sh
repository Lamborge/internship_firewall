#!/bin/bash

mkdir -p bin
clang main.c -o bin/firewall
cp random.sh bin/
cp custom_db.txt bin/
cp intership_db.txt bin/
cp in.txt bin/

echo "Build complete!"
