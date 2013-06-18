require 'numru/netcdf'

$DEBUG = true
include NumRu
file = NetCDF.create_tmp
file.def_dim('x',5)
file.put_att("history", __FILE__ )
p file.path
p file.att("history").get
print "environment variable TEMP ="+(ENV['TEMP'] || '')+"\n"
file2=file
print "000\n"
file.close
GC.start
print "aaa\n"
