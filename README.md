Netcdf-NMatrix
==============

This is a moderate rewrite of the ruby-netcdf package which uses NMatrix instead of NArray, to make it compatible with SciRuby.

It allows reading and writing of data to and from the popular NetCDF binary file format (see the [Unidata](http://www.unidata.ucar.edu/software/netcdf/) website).

It is still a work in progress, fulling supporting attributes and numerical data, but still developing character data capabilities.

Installation
------------

Make sure you have netCDF installed on your system. 

    gem install netcdf-nmatrix 

Example
-------

    require 'netcdf'

    filename = "test.nc"
    file=NumRu::NetCDF.create(filename,false,false)

    dimx=file.def_dim("x",15)
    dimy=file.def_dim("y",10)
    dimz=file.def_dim("z",10)

    batt = file.put_att("type_byte",5,"byte")
    file.put_att("type_short",[222,333,444],"int16")
    file.put_att("type_int",[2222,3333,4444])
    file.put_att("type_float",[2.22,3.33,4.44],"float32")
    file.put_att("type_double",[2.222,3.333,4.444])
    string = file.put_attraw("string","netCDF for Ruby","string")

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


    byte_var.put_var_byte(c)
    int_var.put_var_int(d)
    sfloat_var.put_var_sfloat(e)
    float_var.put_var_float(f)
