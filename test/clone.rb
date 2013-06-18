require 'numru/netcdf'
include NumRu
s = 'tmp.nc'
#f = NetCDF.new(s,'rw')
f = NetCDF.create(s)
f.redef
d = f.def_dim('x',3)
v = f.def_var('x','sfloat',[d])
a = f.put_att('long_name','xx')
f.enddef
v.put([1,2,3])
f.taint
f.freeze
f2 = f.clone
p 'netcdf  clone'
p f.path, f2.path   
p f.tainted?, f2.tainted?, f.frozen?, f2.frozen?  

p 'netcdf  dup'
f3 = f.dup
p f.tainted?, f3.tainted?, f.frozen?, f3.frozen?  

p 'netcdfdim'
d.taint
d2 = d.clone
d3 = d.dup
p d.tainted?, d2.tainted?, d3.tainted?

p 'netcdfvar'
v.taint
v2 = v.clone
v3 = v.dup
p v.tainted?, v2.tainted?, v3.tainted?

p 'netcdfatt'
a.taint
a2 = a.clone
a3 = a.dup
p a.tainted?, a2.tainted?, a3.tainted?

f.close

p 'narray (for reference)'
a = NArray.float(3)
a.taint
b = a.clone
p a.tainted?, b.tainted?  
b = a.dup
p a.tainted?, b.tainted?  





