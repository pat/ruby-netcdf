require "numru/netcdf"
include NumRu

file = NetCDF.create("test.nc")
nx, ny = 10, 5
xdim = file.def_dim("x",nx)
ydim = file.def_dim("y",ny)
tdim = file.def_dim("t",0)
require "date"
file.put_att("history","created by #{$0}  #{Date.today}")

x = file.def_var("x","sfloat",[xdim])
y = file.def_var("y","sfloat",[ydim])
t = file.def_var("t","sfloat",[tdim])
v1 = file.def_var("v1","sfloat",[xdim,ydim])
v1.put_att("long_name","test 1")
v1.put_att("units","1")
v2 = file.def_var("v2","sfloat",[xdim,ydim,tdim])
v2.put_att("long_name","test 2")
v2.put_att("units","1")
file.enddef

x.put( NArray.float(nx).indgen! )
y.put( NArray.float(ny).indgen! )

z = NArray.float(nx,ny).indgen!*0.1
v1.put(z)
v1.put( NArray.float(nx).add!(20), "start"=>[0,2],"end"=>[-1,2])
v2.put(z, "start"=>[0,0,0],"end"=>[-1,-1,0])
t.put( 0, "index"=>[0])
v2.put(-z, "start"=>[0,0,1],"end"=>[-1,-1,1])
t.put( 1, "index"=>[1])

file.close
print `ncdump test.nc`
