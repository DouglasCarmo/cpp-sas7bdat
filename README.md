# CPP - SAS Conversion to CSV

Original Author and all credits: https://github.com/olivia76/cpp-sas7bdat

## Summary

This is a c++ 17 implementation of a SAS7BDAT file reader (Provides a Python interface)

## How to compile and execute code (Docker)
```bash
1. Docker build:
    docker build . -t test
    docker run -it -v <host-volume>:<docker-volumer> test bash
    
2. Inside container:
  cd ~/cpp-sas7bdat
  . ${ACTIVATEPY}
  make
  # convert to CSV           
  ./build/apps/cppsas7bdat-ci convert_csv ./test_package/file.sas7bdat
```

## What is a SAS7BDAT file?

A SAS7BDAT file is a database storage file created by Statistical
Analysis System (SAS) software. It contains a binary encoded dataset
used for advanced analytics, business intelligence, data management,
predictive analytics, and more. (from [fileinfo.com](https://fileinfo.com/extension/sas7bdat))

Different projects already exists to read SAS7BDAT files:

- https://github.com/topics/sas7bdat

This project is based on the following implementations:
- https://github.com/WizardMac/ReadStat/
- https://github.com/tk3369/SASLib.jl
- https://pypi.org/project/sas7bdat/

