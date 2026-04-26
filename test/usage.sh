#!/bin/bash
../build/bin/codegen schema.bs
g++ tester.cpp test.cpp -o test
