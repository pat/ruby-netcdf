require 'numru/netcdf'
include NumRu
s = 'tmp.nc'
f = NetCDF.create(s)
d = f.def_dim('x',2)
v = f.def_var('x','sfloat',[d])
p v.vartype, v.ntype
v = f.def_var('x1','sint',[d])
p v.vartype, v.ntype
v = f.def_var('x2','byte',[d])
p v.vartype, v.ntype

f.close
