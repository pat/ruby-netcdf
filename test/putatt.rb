require 'numru/netcdf'
include NumRu
s = 'tmp.nc'
f = NetCDF.create(s)
d = f.def_dim('x',5)
v = f.def_var('x','sfloat',[d])

require 'date'
att = f.put_att("history", Date.today.to_s )
p att.get, f.att("history").get
att = f.put_att("int_att",123)
att = f.put_att("sfloat_att",1.0/3.0,'sfloat')
att = f.put_att("sfloat_att2",2.0/3.0,NArray::SFLOAT)
att = v.put_att("long_name",'test variable')
att = v.put_att("int_att",123)
p att.get, v.att("int_att").get
att = v.put_att("float_att",1.0/3.0)
att = v.put_att("float_array",[0.1, 0.2, 30])
att = v.put_att("sfloat_narray",NArray.sfloat(3).indgen!/3)
att = v.put_att("float_narray",NArray.float(3).indgen!)
att = v.put_att("sint_narray",NArray.sint(3).indgen!)
att = v.put_att("int2float",10,'float')
att = att = v.put_att("dummy",10,'float')
att.put('changed to text')
att.name = 'changed'
begin
   v.put_att("destined_to_fail",9.8,'complex')
rescue
   print "*1* exception raised as expected -- (",
         __FILE__,":",__LINE__,") ", $!,"\n"
end
begin
   v.put_att("destined_to_fail",9.8,'string')
rescue
   print "*2* exception raised as expected -- (",
         __FILE__,":",__LINE__,") ", $!,"\n"
end

begin
   v.put_att("destined_to_fail",'hello','int')
rescue
   print "*3* exception raised as expected -- (",
         __FILE__,":",__LINE__,") ", $!,"\n"
end

begin
   v.put_att("destined_to_fail",[10,30,'sss'])
rescue
   print "*4* exception raised as expected -- (",
         __FILE__,":",__LINE__,") ", $!,"\n"
end


f.close

print `ncdump tmp.nc`
