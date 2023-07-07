# CPP - SAS Conversion to CSV

Original Author and all credits: https://github.com/olivia76/cpp-sas7bdat

## Summary

This is a c++ 17 implementation of a SAS7BDAT file reader (Provides a Python interface)

## How to compile and execute code (Docker)
```bash
1. Docker build and first:
    docker build . -t test
    docker run -it test bash
    
2. Inside container:
  cd ~/cpp-sas7bdat
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

### Python

3 sinks -- `SinkByRow()`, `SinkByChunk(chunk_size)` and `SinkWholeData()` -- are provided by the
`pycppsas7bdat` python package.  They use `pandas.DataFrame` to store
the data.  


```python
from pycppsas7bdat.read_sas import read_sas

sink = read_sas("filename.sas7bdat", include=[...], exclude=[...])
print(sink.properties)
print(sink.df)
```

```python
from pycppsas7bdat import Reader
from pycppsas7bdat.sink import SinkByRow, SinkByChunk, SinkWholeData

sink = SinkByRow() # or SinkByChunk() or SinkWholeData()   
r = Reader("filename.sas7bdat", sink, include=[...], exclude=[...])
# Read row by row
while r.read_row(): pass

# Read chunk by chunk
while r.read_rows(chunk_size): pass

# OR read the whole file
r.read_all()

# export to pandas.DataFrame
print(sink.properties)
print(sink.df)
```

It is easy to write your own sinks:

```python
class MySink(object):
    rows = []

    def set_properties(self, properties): # This method must be defined
		"""
		@brief: Called once after reading the header and metadata
		@param properties: A Properties object with the header, metadata and columns definition
		"""
        self.columns = [col.name for col in properties.metadata.columns]

    def push_row(self, irow, row): # This method must be defined
		"""
		@brief: Called for every row
		@param irow: Zero-based index of the row
		@param row: A list of value, one for each column.
		"""
        self.rows.append(row)

class MySinkChunk(object):
	chunks = []
	chunk_size = 10000   # This member must be present for a SinkChunk

	def set_properties(self, properties): # This method must be defined
        """
		@brief: Called once after reading the header and metadata
		@param properties: A Properties object with the header, metadata and columns definition
		"""
		self.columns = [col.name for col in properties.metadata.columns]

    def push_rows(self, istartrow, iendrow, rows): # This method must be defined
		"""
		@brief: Called for every read chunk of data
		@param istartrow: Zero-based index for the start row
		@param iendrow: Zero-based index for the end row (included)
		@param rows: A dict of list of values. The keys are the columns'names.
		"""
		chunks.append(rows)
```

