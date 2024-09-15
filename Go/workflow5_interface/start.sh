#!/bin/bash

pkill -f ./workflow5_interface
go build -o workflow5_interface main.go
./workflow5_interface &