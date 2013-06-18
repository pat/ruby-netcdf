require 'numru/netcdf'
include NumRu
file = NetCDF.create("tmp.nc")
var1 = file.def_var_with_dim("var1","sfloat",[6],["x"])
var2 = file.def_var_with_dim("var2","sfloat",[6,3],["x","y"])
var3 = file.def_var_with_dim("var3","sfloat",[3],["y"])
var3 = file.def_var_with_dim("var4","sfloat",[0],["t"])
att = var1.put_att("long_name","test")
file.close

p var1 == nil
p file == nil
p att == nil
p att == att, var1 ==var1, file == file
