require 'numru/netcdf'
include NumRu

s = 'tmp.nc'
f = NetCDF.create(s)

nx = 10
d = f.def_dim('x',nx)
v1 = f.def_var('v1','sint',[d])
v2 = f.def_var('v2','sint',[d])
v3 = f.def_var('v3','int',[d])
v1.put_att('scale_factor',0.1,'sfloat')
v1.put_att('add_offset',100.0,'sfloat')
v2.put_att('scale_factor',0.1,'sfloat')
v2.put_att('add_offset',100.0,'sfloat')
v3.put_att('scale_factor',0.1,'sfloat')
v3.put_att('add_offset',100.0,'sfloat')
f.enddef
v1.put( NArray.sint(nx).indgen!+100 )
v2.scaled_put( NArray.float(nx).indgen!+100 )
v3.scaled_put( NArray.float(nx).indgen!+100 )
f.close


print "** originally unscaled\n"
f = NetCDF.open(s)
v1 = f.var('v1')
v11 = v1.get
v12 = v1.scaled_get
p v11
p v12

print "** originally scaled\n"
v2 = f.var('v2')
v21 = v2.get
v22 = v2.scaled_get
p v21
p v22

print "** originally sclaed (int --> double)\n"
v3 = f.var('v3')
v31 = v3.get
v32 = v3.scaled_get
p v31
p v32

print "** unpack type fixed to sfloat\n"
NetCDFVar.unpack_type = NArray::SFLOAT
v33 = v3.scaled_get
p v33
NetCDFVar.unpack_type = NArray::INT
v33 = v3.scaled_get
p v33
