#!/bin/bash

FILE="findSensor"

g++ $FILE.cpp `pkg-config --cflags --libs opencv` -o $FILE
