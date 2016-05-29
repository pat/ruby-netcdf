##require 'numru/netcdf' 
## // to test before make install -->
require 'nmatrix'
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
file.put_att("type_short",[222,333,444],"int16")
file.put_att("type_int",[2222,3333,4444])
file.put_att("type_float",[2.22,3.33,4.44],"float32")
file.put_att("type_double",[2.222,3.333,4.444])
string = file.put_attraw("string","netCDF for Ruby","string")
batt.put(6,"byte")

int16_var=file.def_var("test_int16","int16",["x"])
byte_var=file.def_var("test_byte","byte",["y"])
byte_var2=file.def_var("test_byte2","byte",[dimy,dimz])
int_var=file.def_var("test_int","int32",["y"])
sfloat_var=file.def_var("test_sfloat","float32",["z"])
float_var=file.def_var("test_float","float64",["y"])

a=NMatrix.seq([10], dtype: :int16)
b=NMatrix.ones([10], dtype: :int16) * 7.0
c=NMatrix.bindgen([10])
d=NMatrix.seq([10], dtype: :int32)
e=NMatrix.ones([10], dtype: :float32) * 1.111
f=NMatrix.ones([10], dtype: :float64) * 5.5555555
file.enddef
file2 = file
(file2 == file)


byte_var.put_var_byte(c)
int_var.put_var_int(d)
sfloat_var.put_var_sfloat(e)
float_var.put_var_float(f)

file.redef
byte_vara=file.def_var("test_byte_vara","byte",[dimy,dimz]);
int16_vara=file.def_var("test_int16_vara","int16",["y","z"]);
int_vara=file.def_var("test_int_vara","int32",["y","z"]);
sfloat_vara=file.def_var("test_sfloat_vara","float32",["y","z"]);
float_vara=file.def_var("test_float_vara","float64",["y","z"]);
file.enddef

byte_vara2 = byte_vara
(byte_vara2 == byte_vara)

g=NMatrix.bindgen([10,10])
h=NMatrix.bindgen([2,3])
gh=NMatrix.ones([1,1], dtype: :byte) * 33
k=NMatrix.seq([10,10], dtype: :int16)
l=NMatrix.seq([2,3], dtype: :int16)
kl=NMatrix.ones([1,1], dtype: :int16) * 44
m=NMatrix.seq([10,10], dtype: :int32)
n=NMatrix.seq([2,3], dtype: :int32)
mn=NMatrix.ones([1,1], dtype: :int32) * 55
o=NMatrix.ones([10,10], dtype: :float32) * 1.234567
p=NMatrix.ones([2,3], dtype: :float32) * 2.345678
op=NMatrix.ones([1,1], dtype: :float32) * 3.4
q=NMatrix.ones([10,10], dtype: :float64) * 1.234
r=NMatrix.ones([2,3], dtype: :float64) * 2.345
qr=NMatrix.ones([1,1], dtype: :float64) * 4.5
s=NMatrix.ones([2,2], dtype: :float64) * 10.0



byte_vara.put(g)
byte_vara.put(h,{"start"=>[3,5]})
byte_vara.put(g,{"start"=>[0,0],"end"=>[9,9]})
byte_vara.put(h,{"start"=>[-8,2]})
byte_vara.put(h,{"start"=>[0,0],"stride"=>[2,3]})
byte_vara.put(h,{'start'=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
byte_vara.put(gh,{"index"=>[4,7]})

int16_vara.put(k)
int16_vara.put(l,{"start"=>[3,5]})
int16_vara.put(k,{"start"=>[0,0],"end"=>[9,9]})
int16_vara.put(l,{"start"=>[-8,2]})
int16_vara.put(l,{"start"=>[0,0],"stride"=>[2,3]})
int16_vara.put(l,{"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
int16_vara.put(kl,{"index"=>[4,7]})

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

na_bba=int16_vara.get({"start"=>[3,5]})
na_bbb=int16_vara.get({"start"=>[0,0],"end"=>[9,9]})
na_bbc=int16_vara.get({"start"=>[-8,2]})
na_bbd=int16_vara.get({"start"=>[0,0],"stride"=>[2,3]})
na_bbf=int16_vara.get({"start"=>[1,1],"end"=>[3,7],"stride"=>[2,3]})
na_bbg=int16_vara.get({"index"=>[4,7]})
na_bbh=int16_vara.get

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

copy_int16 = string.copy(int16_vara)
copy_int16.put("%%%%%")
copy_int = string.copy(int_vara)
copy_int.put([0,60],"int32")
copy_sfloat = string.copy(sfloat_vara)
copy_sfloat.put([0.01,5.5],"float32")
copy_float = string.copy(float_vara)
copy_float.put([0.0001,5.5555],"float64")
file.enddef
nm = copy_byte.name
att0 = string.get
att1 = copy_byte.get
att2 = copy_int16.get
att3 = copy_int.get
att4 = copy_sfloat.get
att5 = copy_float.get
file.fill(true)
file.fill(false)

float_vara.dim_names
float_vara.att_names

file.close
exit(0)

