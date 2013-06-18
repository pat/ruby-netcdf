## test of "char" variables

require 'numru/netcdf'
include NumRu
s = 'tmp.nc'
f = NetCDF.create(s)
d = f.def_dim('x',5)
v = f.def_var('x','char',[d])
tv = f.def_var('text','char',[d])
f.enddef
v.put( NArray.byte(5).indgen! )
tv.put( NArray.to_na("hello","byte",5) )
tv.put( NArray.to_na("LO","byte",2), 'start'=>[3] )
tv.put( NArray.to_na("H","byte",1), 'index'=>[0] )
f.close

f = NetCDF.open(s)
v = f.var('x')
p v.get
tv = f.var('text')
p tv.get
p tv.get.to_s



