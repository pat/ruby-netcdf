require 'numru/netcdf'
include NumRu
s = 'tmp.nc'
f = NetCDF.create(s)
f.redef
dx = f.def_dim('x',5)
dy = f.def_dim('y',3)
v = f.def_var('x','sfloat',[dx])
v2 = f.def_var('x2',NArray::SFLOAT,[dx])
vxy = f.def_var('xy','sfloat',[dx,dy])
f.enddef
v.put([1,2,3,4,5])
p 'testing []= ...'
v[{0..3=>2}]=[100,500]
v[1]=999
v[3..4]=[-99,-99]
v2.put(666)
v2[0..2]=777
vxy.put(NArray.sfloat(5,3).indgen!)
vxy[[2,0],[0,2,1]] = [[1,2],[3,4],[5,6]]
vxy[1,[2,0,1]] = [10,20,30]
vxy[[4,3],2] = [100,200]

f.close

f = NetCDF.open(s)
v = f.var('x')
p 'testing [] ...'
p '*0*',v[{0..3=>2}]
p '*1*',v[1]
p '*2*',v[3..4],v.rank
p '*3*',v[[2,0,0]]
vxy = f.var('xy')
p '*4*',vxy[[2,0],[0,2,1]]
p '*5*',vxy[1,[2,0,1]]
p '*6*',vxy[[4,3],2]
f.close
