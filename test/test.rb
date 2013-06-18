##require 'numru/netcdf' 
## // to test before make install -->
require 'narray'
require '../netcdfraw'  
require '../lib/netcdf'
## <-- to test before make install //

include NumRu

filename = "test.nc"
print "creating ",filename,"...\n"
file=NetCDF.create(filename,false,false)

dimx=file.def_dim("x",15)
dimy=file.def_dim("y",10)
dimz=file.def_dim("z",10)

batt = file.put_att("type_byte",5,"byte")
file.put_att("type_short",[222,333,444],"sint")
file.put_att("type_int",[2222,3333,4444])
file.put_att("type_float",[2.22,3.33,4.44],"sfloat")
file.put_att("type_double",[2.222,3.333,4.444])
string = file.put_attraw("string","netCDF for Ruby","string")
batt.put(6,"byte")

sint_var=file.def_var("test_sint","sint",["x"])
byte_var=file.def_var("test_byte","byte",["y"])
byte_var2=file.def_var("test_byte2","byte",[dimy,dimz])
int_var=file.def_var("test_int","int",["y"])
sfloat_var=file.def_var("test_sfloat","sfloat",["z"])
float_var=file.def_var("test_float","float",["y"])

a=NArray.sint(10).indgen
b=NArray.sint(10).fill(7)
c=NArray.byte(10).indgen
d=NArray.int(10).indgen
e=NArray.sfloat(10).fill(1.111)
f=NArray.float(10).fill(5.5555555)
file.enddef
file2 = file
(file2 == file)

byte_var.put_var_byte(c)
int_var.put_var_int(d)
sfloat_var.put_var_sfloat(e)
float_var.put_var_float(f)

file.redef
byte_vara=file.def_var("test_byte_vara","byte",[dimy,dimz]);
sint_vara=file.def_var("test_sint_vara","sint",["y","z"]);
int_vara=file.def_var("test_int_vara","int",["y","z"]);
sfloat_vara=file.def_var("test_sfloat_vara","sfloat",["y","z"]);
float_vara=file.def_var("test_float_vara","float",["y","z"]);
file.enddef

byte_vara2 = byte_vara
(byte_vara2 == byte_vara)

g=NArray.byte(10,10).indgen
h=NArray.byte(2,3).indgen
gh=NArray.byte(1,1).fill(33)
k=NArray.sint(10,10).indgen
l=NArray.sint(2,3).indgen
kl=NArray.sint(1,1).fill(44)
m=NArray.int(10,10).indgen
n=NArray.int(2,3).indgen
mn=NArray.int(1,1).fill(55)
o=NArray.sfloat(10,10).fill(1.234567)
p=NArray.sfloat(2,3).fill(2.345678)
op=NArray.int(1,1).fill(3.4)
q=NArray.float(10,10).fill(1.234)
r=NArray.float(2,3).fill(2.345)
qr=NArray.float(1,1).fill(4.5)
s=NArray.float(2,2).fill(10.0)


byte_vara.put(g)
byte_vara.put(h,{"start"=>[3,5]})
byte_vara.put(g,{"start"=>[0,0],"end"=>[9,9]})
byte_vara.put(h,{"start"=>[-8,2]})
byte_vara.put(h,{"start"=>[0,0],"stride"=>[2,3]})
byte_vara.put(h,{'start'=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
byte_vara.put(gh,{"index"=>[4,7]})

sint_vara.put(k)
sint_vara.put(l,{"start"=>[3,5]})
sint_vara.put(k,{"start"=>[0,0],"end"=>[9,9]})
sint_vara.put(l,{"start"=>[-8,2]})
sint_vara.put(l,{"start"=>[0,0],"stride"=>[2,3]})
sint_vara.put(l,{"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
sint_vara.put(kl,{"index"=>[4,7]})

int_vara.put(m)
int_vara.put(n,{"start"=>[3,5]})
int_vara.put(m,{"start"=>[0,0],"end"=>[9,9]})
int_vara.put(n,{"start"=>[-8,2]})
int_vara.put(n,{"start"=>[0,0],"stride"=>[2,3]})
int_vara.put(n,{"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
int_vara.put(mn,{"index"=>[4,7]})

sfloat_vara.put(o)
sfloat_vara.put(p,{"start"=>[3,5]})
sfloat_vara.put(o,{"start"=>[0,0],"end"=>[9,9]})
sfloat_vara.put(p,{"start"=>[-8,2]})
sfloat_vara.put(p,{"start"=>[0,0],"stride"=>[2,3]})
sfloat_vara.put(p,{"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
sfloat_vara.put(op,{"index"=>[4,7]})

float_vara.put(q)
float_vara.put(r,{"start"=>[3,5]})
float_vara.put(q,{"start"=>[0,0],"end"=>[9,9]})
float_vara.put(r,{"start"=>[-8,2]})
float_vara.put(r,{"start"=>[0,0],"stride"=>[2,3]})
float_vara.put(r,{"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
float_vara.put(qr,{"index"=>[4,7]})

float_vara.dim_names

na_aaa=byte_vara.get({"start"=>[3,5]})
na_aab=byte_vara.get({"start"=>[0,0],"end"=>[9,9]})
na_aac=byte_vara.get({"start"=>[-8,2]})
na_aad=byte_vara.get({"start"=>[0,0],"stride"=>[2,3]})
na_aae=byte_vara.get({'start'=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
na_aaf=byte_vara.get({"index"=>[4,7]})
na_aag=byte_vara.get

na_bba=sint_vara.get({"start"=>[3,5]})
na_bbb=sint_vara.get({"start"=>[0,0],"end"=>[9,9]})
na_bbc=sint_vara.get({"start"=>[-8,2]})
na_bbd=sint_vara.get({"start"=>[0,0],"stride"=>[2,3]})
na_bbf=sint_vara.get({"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
na_bbg=sint_vara.get({"index"=>[4,7]})
na_bbh=sint_vara.get

na_cca=int_vara.get({"start"=>[3,5]})
na_ccb=int_vara.get({"start"=>[0,0],"end"=>[9,9]})
na_ccc=int_vara.get({"start"=>[-8,2]})
na_ccd=int_vara.get({"start"=>[0,0],"stride"=>[2,3]})
na_cce=int_vara.get({"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
na_ccf=int_vara.get({"index"=>[4,7]})
na_ccg=int_vara.get

na_dda=sfloat_vara.get({"start"=>[3,5]})
na_ddb=sfloat_vara.get({"start"=>[0,0],"end"=>[9,9]})
na_ddc=sfloat_vara.get({"start"=>[-8,2]})
na_ddd=sfloat_vara.get({"start"=>[0,0],"stride"=>[2,3]})
na_dde=sfloat_vara.get({"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
na_ddf=sfloat_vara.get({"index"=>[4,7]})
na_ddg=sfloat_vara.get

na_eea=float_vara.get({"start"=>[3,5]})
na_eeb=float_vara.get({"start"=>[0,0],"end"=>[9,9]})
na_eec=float_vara.get({"start"=>[-8,2]})
na_eed=float_vara.get({"start"=>[0,0],"stride"=>[2,3]})
na_eee=float_vara.get({"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
na_eef=float_vara.get({"index"=>[4,7]})
na_eeg=float_vara.get

file.redef

copy_byte = string.copy(byte_vara) 
copy_byte.put([0,20],"byte") 
copy_byte.name="new_name"

copy_byte2 = copy_byte
(copy_byte2 == copy_byte)

copy_sint = string.copy(sint_vara)
copy_sint.put("%%%%%")
copy_int = string.copy(int_vara)
copy_int.put([0,60],"int")
copy_sfloat = string.copy(sfloat_vara)
copy_sfloat.put([0.01,5.5],"sfloat")
copy_float = string.copy(float_vara)
copy_float.put([0.0001,5.5555],"float")
file.enddef
nm = copy_byte.name
att0 = string.get
att1 = copy_byte.get
att2 = copy_sint.get
att3 = copy_int.get
att4 = copy_sfloat.get
att5 = copy_float.get
file.fill(true)
file.fill(false)

float_vara.dim_names
float_vara.att_names

file.close
exit(0)

