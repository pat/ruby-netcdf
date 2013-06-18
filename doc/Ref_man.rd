=begin
=RubyNetCDF Reference Manual

* ((<Method Index>))

---------------------------------------------

==Overview

RubyNetCDF is the Ruby interface of the NetCDF library. Ruby is a free 
object-oriented scripting language and is freely available from
((<the Ruby homepage|URL:http://www.ruby-lang.org/>)). 
To handle numeric data, RubyNetCDF uses
((<NArray|URL:http://www.ruby-lang.org/en/raa-list.rhtml?name=NArray>)), which is the standard numeric multi-dimensional array class
for Ruby. Thus, you have to have installed it before installing this library.
An NArray object holds numeric data in a consecutive memory area
pointed by a C pointer. Thus, it is computationally efficient.
NArray is similar to NumPy for Python, but results of some benchmark
tests suggests that NArray is more efficient than NumPy.
Optionally, RubyNetCDF offers methods to handle data missing 
automatically. To use it, you will also
need ((<NArrayMiss|URL:http://ruby.gfd-dennou.org/products/narray_miss/>)). 
See ((<Usage>)) for details.

===Structure

RubyNetCDF consists of the four classes in the following.

* ((<class NetCDF>)) -- the file class

  An NetCDF object represents a NetCDF file

* ((<class NetCDFDim>)) -- the dimension class

  Although in its C version a NetCDF dimension is represented by a
  combination of a file ID and a dimension ID, it is represented by
  only one NetCDFDim object in RubyNetCDF.

* ((<class NetCDFVar>)) -- the variable class

  Although in its C version a NetCDF variable is represented by a
  combination of a file ID and a variable ID, it is represented by
  only one NetCDFVar object in RubyNetCDF.

* ((<class NetCDFAtt>)) -- the attribute class

  Although in its C version a NetCDF attribute is represented by a
  combination of file ID, variable ID, and its name, it is represented
  by only one NetCDFAtt object in RubyNetCDF.

===Data type

All the NetCDF variable types char, byte, short, int, float, and
double are supported in this Ruby interface. These types are called,
however, differently in it to adhere to the convention of Ruby, or,
more specifically, of NArray. These types are named to as "char",
"byte", "sint", "int", "sfloat", and "float", respectively. Therefore,
the vartype (=ntype) method of the NetCDFVar class returns one of these
strings.  The def_var method of the NetCDF class also accepts one of
them to define a variable.  It should be noted especially that "float"
in this library means the double in the NetCDF terminology. This is
due to the convention of Ruby -- the predefined Float class
corresponds to the double in C, not the float.

The "get" method of NetCDFVar class reads a variable in a NArray of
the same type as in the file, except for the "char" type which is read
into a "byte".  This is because NArray does not have a "char" type.
However, it not is not supposed to be a problem, since a byte NArray
can easily handle string data.


===Error handling

Errors are basically handled by raising exceptions. However, light
errors in value-returning methods are handled by returning nil (e.g.,
if a non-existent attribute name is specified in attribute reading).
Those methods that return nil on error are explicitly written as such 
in the following.

===Security features

Security handling is done just as in the pre-defined File class.

===Usage

To use the RubyNetCDF library, load the library first by placing the
following line in your Ruby program to load the library:

   require 'numru/netcdf'

If you want to use automatic data-missing-handling methods
(of NetCDFVar class), use the following:

   require 'numru/netcdf_miss'

This will call (({require 'numru/netcdf'})) inside at the beginning, so 
you do not have to call the both. The missing-data handling is done 
with ((<NArrayMiss|URL:http://ruby.gfd-dennou.org/products/narray_miss/>)),
so you have have installed it. This is, however, not needed if you only
call (({require 'numru/netcdf'})).

Here, 'numru', which stands for "Numerical Ruby", is the name of
the subdirectory in the user's load path where the RubyNetCDF library
is placed. Then, it can be used as in the following:

   file = NumRu::NetCDF.create('tmp.nc')
   x = file.def_dim('x',10)
   y = file.def_dim('y',10)
   v = file.def_var('v','float',[x,y])
   file.close

Here, NumRu is the module that has the library in it. The
reason why NetCDF library is wrapped in such a module is to avoid
conflicts in the name space. Without this kind of treatment,
problems happen if the user wants to use a library that happens to
have a class or module with the same name as even one of the classes
in this library.

If such a problem is not expected to happen, the prefix "NumRu::" can
be eliminated by "including" the NumRu module as in the following, so
that to place "NumRu::" is not needed anymore in the current scope:

   include NumRu
   file = NetCDF.create('tmp.nc')
   ...

For more examples, see demo and test programs included in the
distribution package.

---------------------------------------------

==How to read this manual

--- method_name(argument1, argument2, ...) -- arguments that can be omitted are expressed as Argument_name=Default_value

     Explanation of its function

     Arguments
     * name of argument1 (its class or possible values): explanation
     * name of argument2 (its class or possible values): explanation
     * ...

     Return value
     * Explanation of the return value

     Corresponding (dependent) function(s) in the C library of NetCDF
     * Name(s) in NetCDF ver 3. The function equivalent to the current
       method, if not in parenthesis. If no direct correspondence,
       dependent functions are listed in parentheses.

---------------------------------------------

==Method Index
* ((<class NetCDF>))

  Class Methods
    * ((<NetCDF.open>))     Opens a file (class method). If mode="w" and non-existent, a new
    * ((<NetCDF.new>))     Aliased to NetCDF.open
    * ((<NetCDF.create>))     Creates a NetCDF file (class method)
    * ((<NetCDF.create_tmp>))     Creates a temporary NetCDF file (class method)

  Instance Methods
    * ((<close>))     Closes the file.
    * ((<ndims>))     Returns the number of dimensions in the file
    * ((<nvars>))     Returns the number of variables in the file
    * ((<natts>))     Returns the number of global attributes in the file
    * ((<unlimited>))     Returns the unlimited dimension in the file
    * ((<path>))     Returns the path of the file (contents of the filename specified when opened/created)
    * ((<redef>))     Switches to the define mode. Does nothing if already in it (nil returned).
    * ((<enddef>))     Switches to the data mode. Does nothing if already in it (nil returned).
    * ((<define_mode?>))     Inquire whether the file is in the define mode.
    * ((<sync>))     Synchronizes the disk copy of a netCDF dataset with in-memory buffer
    * ((<def_dim>))     Define a dimension
    * ((<put_att>))     Sets a global attribute
    * ((<def_var>))     Defines a variable
    * ((<def_var_with_dim>))     Same as def_var but defines dimensions first if needed
    * ((<var>))     Opens an existing variable in the file
    * ((<vars>))    Opens existing variables in the file
    * ((<dim>))     Opens an existing dimension in the file
    * ((<dims>))     Opens existing dimensions in the file
    * ((<att>))     Opens an existing global attribute in the file
    * ((<fill=>))     Sets a fill mode. (Default behavior of NetCDF is FILL.)
    * ((<each_dim>))     Iterator regarding the dimensions in the file.
    * ((<each_var>))     Iterator regarding the variables in the file.
    * ((<each_att>))     Iterator regarding the global attributes of the file.
    * ((<dim_names>))     Returns the names of all dimensions in the file
    * ((<var_names>))     Returns the names of all variables in the file
    * ((<att_names>))     Returns the names of all the global attributes of the file

* ((<class NetCDFDim>))

  Class Methods

  Instance Methods
    * ((<length>))     Returns the length of the dimension
    * ((<length_ul0>))     Same as length but returns 0 for the unlimited dimension
    * ((<name=>))     Rename the dimension
    * ((<name>))     Returns the name of the  dimension
    * ((<unlimited?>))     Inquires whether the dimension is unlimited or not

* ((<class NetCDFVar>))

  Class Methods
    * ((<NetCDFVar.new>))     Combines NetCDF.open and NetCDF#Var to open a variable with one line (no need to use this).
    * ((<NetCDFVar.unpack_type=>))    Fix the NArray type to be used
      in ((<unpack>))
    * ((<NetCDFVar.unpack_type>))     Returns the NArray type set by ((<NetCDFVar.unpack_type=>)).

  Instance Methods
    * ((<dim>))     Inquires the dim_num-th dimension of the variable (dim_num=0,1,2,..)
    * ((<dims>))     Returns an array of all the dimensions of the variable
    * ((<shape_ul0>))     Returns the shape of the variable, but the length of the unlimited dimension is set to zero.
    * ((<shape_current>))     Returns the current shape of the variable.
##    * ((<url>))     Returns a combination of filename (path) and variable name as path+'?var='+varname
    * ((<each_att>))     Iterator regarding the global attributes of the variables.
    * ((<dim_names>))     Returns the names of all the dimensions of the variable
    * ((<att_names>))     Returns the names of all the attributes of the variable
    * ((<name>))     Returns the name of the variable
    * ((<name=>))     Rename the variable
    * ((<ndims>))     Number of dimensions of the variable (which is rank of the variable).
    * ((<rank>))     aliased to ndims
    * ((<ntype>))      Aliased to vartype
    * ((<vartype>))     Inquires the data value type of the variable
    * ((<typecode>))     Inquires the data type of the variable (returns a typecode of NArray)
    * ((<natts>))     Returns the number of the attributes of the variable
    * ((<file>))     Inquires the file that the variable is in
    * ((<att>))     Returns the attribute specified by its name
    * ((<put_att>))     Sets an attribute
    * ((<put>))     Aliased to ((<simple_put>))
    * ((<simple_put>))     Set the values of the variable
    * ((<pack>)) Pack a NArray (etc) using the attributes scale_factor and/or add_offset of self.
    * ((<scaled_put>))     Same as ((<simple_put>)) but interprets the attributes scale_factor and/or add_offset using ((<pack>)).
    * ((<get>))     Aliased to ((<simple_get>))
    * ((<simple_get>))     Returns values of the variable
    * ((<unpack>)) Unpack a NArray (etc) using the attributes scale_factor and/or add_offset of self.
    * ((<scaled_get>))     Same as ((<simple_get>)) but interprets the attributes scale_factor and/or add_offset using ((<unpack>)).
    * ((<[]>))     Same as NetCDFVar#get but a subset is specified as in the method [] of NArray. 
    * ((<[]=>))     Same as NetCDFVar#put but a subset is specified as in the method []= of NArray. 


  Instance Methods added by requiring "numru/netcdf_miss"

    * ((<get_with_miss>))     Same as ((<get>)) but interprets data missing.
    * ((<get_with_miss_and_scaling>))     Same as ((<get_with_miss>)) but handles data scaling too.
    * ((<put_with_miss>))     Same as ((<put>)) but interprets data missing.
    * ((<put_with_miss_and_scaling>))     Same as ((<put_with_miss>)) but handles data scaling too.


* ((<class NetCDFAtt>))

  Class Methods

  Instance Methods
    * ((<name>))     Returns the name of the attribute
    * ((<name=>))     Rename the attribute
    * ((<copy>))     Copies an attribute to a variable or a file. If file, becomes an global attribute
    * ((<delete>))     Delete an attribute
    * ((<put>))     Sets the value of the attribute
    * ((<get>))     Returns the values of the attribute
    * ((<atttype>))     Inquires the type of attribute values
    * ((<typecode>))    Inquires the type of attribute values (returns a NArray typecode)

---------------------------------------------

=class NetCDF
===Class Methods
---NetCDF.open(filename, mode="r", share=false)
     Opens a file (class method). If mode="w" and the file does not
     exist, a new file is created.

     Arguments
     * filename (String): file name (path)
     * mode (String) : IO mode "r" (read only); "w","w+" (write --
       current contents are overwritten (eliminated!)); "r+","a","a+"
       (append -- writable while current contents are preserved).
       All the options permit reading, unlike the predefined File class.
       Note that to "append" will require extra time and disk
       space due to the limitations of the original NetCDF library,
       which is used in this library.
     * share (true or false) : Whether to use the "shared" mode or not 
       (set true if a file being written may be read from other
       processes. See nc_open in Ch.5 of users' guide of the C version)

     Return value
     * a NetCDF object

     Corresponding (dependent) function(s) in the C library of NetCDF 
     * nc_open, nc_create

---NetCDF.new
     Aliased to NetCDF.open

---NetCDF.create(filename, noclobber=false, share=false)
     Creates a NetCDF file (class method)
     
     Arguments
     * filename (String) : file name (path)
     * noclobber (true or false) : overwrite or not if the file exists
     * share (true or false) : Whether to use the shared mode or not
       (set true if a file being written may be read from other
       processes. See nc_open in Ch.5 of users' guide of the C version)

     Return value
     *  a NetCDF object

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_create

---NetCDF.create_tmp(tmpdir=ENV['TMPDIR']||ENV['TMP']||ENV['TEMP']||'.', share=false)
     Creates a temporary NetCDF file (class method).
     Its name is automatically generated, and it is deleted when closed.
     
     Arguments
     * tmpdir (String) : directory to place the temporary file.
       By default, "." or a directory specified by an environmental
       variable (TMPDIR or TMP or TEMP) is used. In a secure mode,
       theses environmental variable is NOT used, and the default
       value is '.'.
     * share (true or false) : Whether to use the shared mode or not

     Return value
     *  a NetCDF object

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_create

===Instance Methods
---close
     Closes the file.

     Arguments
     *  (none)

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_close

---ndims
     Returns the number of dimensions in the file

     Arguments
     *  (none)

     Return value
     *  Integer

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_ndims

---nvars
     Returns the number of variables in the file
   
     Arguments
     *  (none)

     Return value
     *  Integer
     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_nvars

---natts
     Returns the number of global attributes in the file

     Arguments
     *  (none)

     Return value
     *  Integer

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_natts

---unlimited
     Returns the unlimited dimension in the file

     Arguments
     *  (none)

     Return value
     *  a NetCDFDim if it exists; nil if not

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_unlimdim

---path
     Returns the path of the file (contents of the filename specified when opened/created)

     Arguments
     *  (none)

     Return value
     *  String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (none)

---redef
     Switches to the define mode. Does nothing if already in it (nil returned).

     Arguments
     *  (none)

     Return value
     *  true if successfully switched to the define mode;
	nil if the file is already in the define mode.
	Exception is raised if unsuccessful for other reasons.

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_redef

---enddef
     Switches to the data mode. Does nothing if already in it (nil returned).

     Arguments
     *  (none)

     Return value
     *  true if successfully switched to the data mode;
	nil if the file is already in the data mode.
	Exception is raised if unsuccessful for other reasons.

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_enddef

---define_mode?
     Inquire whether the file is in the define mode.

     Arguments
     *  (none)

     Return value
     *  true if the data is in the define mode;
	false if the file is in the data mode;
	nil otherwise (possibly the file is read-only).

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  combination of nc_redef and nc_enddef

---sync
     Synchronizes the disk copy of a netCDF dataset with in-memory buffer

     Arguments
     *  (none)

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_sync

---def_dim(dimension_name, length)
     Define a dimension

     Arguments
     * dimension_name (String) : Name of the dimension to be defined
     * length (Integer) : length of the dimension. 0 for unlimited.

     Return value
     *  defined dimension (NetCDFDim object)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_def_dim

---put_att(attribute_name, value, atttype=nil)
     Sets a global attribute

     Arguments
     * attribute_name (String) : name of the global attribute
     * value (Numeric, String, Array of Numeric, or NArray) : value of the attribute
     * atttype (nil or String) : data type of the attribute value.
       nil lets it automatically determined from the value.
       "char" (or "string"), "byte", "sint", "int", "sfloat", or "float"
       specifies the type explicitly (1,1,2,4,4,8 bytes, respectively)

     Return value
     *  created attribute (NetCDFAtt object)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_put_att_<type>

---def_var(name, vartype, dimensions)
     Defines a variable

     Arguments
     * name (String) : Name of the variable to define
     * vartype (String or Fixnum) : data type of the variable ("char", "byte", "sint",
       "sint", "int", "sfloat", or "float"), or a NArray typecodes(Fixnum)
     * dimensions (Array) : Dimensions of the variable. An Array of
       NetCDFDim, in the order from the fastest varying dimension to
       the slowest varying one; its length becomes the rank of the
       variable.

     Return value
     *  defined variable (NetCDFVar object)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_def_var

---def_var_with_dim(name, vartype, shape_ul0, dimnames)
     Same as def_var but defines dimensions first if needed.
     Raise exception if it conflicts with the lengths of existing dimensions.

     Arguments
     * name (String) : Name of the variable to define
     * vartype (String) : data type of the variable ("char", "byte", "sint",
       "sint", "int", "sfloat", or "float")
     * shape_ul0 (Array of Integer) : Shape of the variable, i.e.,
       lengths of dimensions. The unlimited dimension is specified by zero.
       The length of shape_ul0 determines the rank of the variable.
     * dimnames (Array of String) : Names of the dimensions. Its length
       (=>rank) must be equal to that of shape_ul0

     Return value
     *  defined variable (NetCDFVar object)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (nc_def_var)

---var(var_name)
     Opens an existing variable in the file

     Arguments
     * var_name (String) : Name of the variable to open

     Return value
     * a NetCDFVar object; nil if the variable does not exist

     Corresponding (dependent) function(s) in the C library of NetCDF
     * nc_inq_varid

---vars(names)
     Opens existing variables in the file

     Arguments
     * names (nil or Array of String): Names of the variables to open; 
       all variables are returned if nil (default).

     Return value
     * Array of NetCDFVar objects; exception is raised if names has a 
       non-existent name

     Corresponding (dependent) function(s) in the C library of NetCDF
     * nc_inq_varid

---dim(dimension_name)
     Opens an existing dimension in the file

     Arguments
     * dimension_name (String) : Name of the dimension to open

     Return value
     * a NetCDFDim object; nil if the dimension does not exist

     Corresponding (dependent) function(s) in the C library of NetCDF
     * nc_inq_dimid

---dims(names)
     Opens existing dimensions in the file

     Arguments
     * names (nil or Array of String): Names of the dimensions to open; 
       all dimensions are returned if nil (default).

     Return value
     * Array of NetCDFDim objects; exception is raised if names has a 
       non-existent name

     Corresponding (dependent) function(s) in the C library of NetCDF
     * nc_inq_dimid

---att(attribute_name)
     Opens an existing global attribute in the file

     Arguments
     * attribute_name (String) : Name of the global attribute to open

     Return value
     *  a NetCDFAtt object if the attribute exists; nil if not

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (nc_inq_attid used for inquiry)

---fill=(filemode)
     Sets a fill mode. (Default behavior of NetCDF is FILL.)

     Arguments
     * fillmode (true or false)

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_set_fill

---each_dim{ ... }
     Iterator regarding the dimensions in the file.
     Ex.: {|i| print i.name,"\n"} prints names of all dimensions

     Arguments
     * { ... } : Block for the iterator. A "do end" block is the alternative.

     Return value
     *  self

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_ndims)

---each_var{ ... }
     Iterator regarding the variables in the file.
     Ex.: {|i| print i.name,"\n"} prints names of all variables

     Arguments
     * { ... } :  Block for the iterator. A "do end" block is the alternative.

     Return value
     *  self

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_nvars)

---each_att{ ... }
     Iterator regarding the global attributes of the file.
     Ex.: {|i| print i.name,"\n"} prints names of all of them.

     Arguments
     * { ... } : Block for the iterator. A "do end" block is the alternative.

     Return value
     *  self

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_natts, nc_inq_attname)

---dim_names
     Returns the names of all dimensions in the file

     Arguments
     *  (none)

     Return value
     *  Array of NetCDFDim

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (nc_inq_ndims, nc_inq_dimname)

---var_names
     Returns the names of all variables in the file

     Arguments
     *  (none)

     Return value
     *  Array of String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_nvars, nc_inq_varname)

---att_names
     Returns the names of all the global attributes of the file

     Arguments
     *  (none)

     Return value
     *  Array of NetCDFAtt

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_natts, nc_inq_attname)

---------------------------------------------

=class NetCDFDim
===Class Methods

===Instance Methods
---length
     Returns the length of the dimension

     Arguments
     *  (none)

     Return value
     *  Integer

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_dimlen

---length_ul0
     Same as length but returns 0 for the unlimited dimension

     Arguments
     *  (none)

     Return value
     *  Integer

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_dimlen

---name=(dimension_newname)
     Rename the dimension

     Arguments
     * dimension_newname (String) : new name

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_rename_dim

---name
     Returns the name of the  dimension

     Arguments
     *  (none)

     Return value
     *  String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_dimname

---unlimited?
     Inquires whether the dimension is unlimited or not

     Arguments
     *  (none)

     Return value
     *  true or false

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_unlimdim)

---------------------------------------------

=class NetCDFVar
===Class Methods
---NetCDFVar.new(file,varname,mode="r",share=false)
     open a NetCDF variable. This can also be done with NetCDF#var
     (instance method of NetCDF class),  which is recommended over
     this method.

     Arguments
     * file (NetCDF or String) : a NetCDF file object (NetCDF)
       or the path of a NetCDF file (String).
     * varname (String) : name of the variable in the file
     * mode (String) : IO mode -- used if file is a String (see NetCDF.open)
     * share (true or false) : Whether to use the "shared" mode or 
       not  -- used if file is a String (see NetCDF.open)

     Return value
     * a NetCDFVar object

     Corresponding (dependent) function(s) in the C library of NetCDF 
     * (dependent on nc_open, nc_create, nc_inq_varid etc.)

---NetCDFVar.unpack_type=(na_type)
     Fix the NArray type to be used in ((<unpack>)).

     Arguments
     * na_type (Integer) : NArray::BYTE, NArray::SINT, NArray::INT, 
       NArray::SFLOAT, or NArray::FLOAT

     Return value
     * na_type (the argument)

---NetCDFVar.unpack_type
     Returns the NArray type set by ((<NetCDFVar.unpack_type=>)).

     Return value
     * nil, NArray::BYTE, NArray::SINT, NArray::INT, 
       NArray::SFLOAT, or NArray::FLOAT

===Instance Methods
---dim(dim_num)
     Inquires the dim_num-th dimension of the variable (dim_num=0,1,2,..)

     Arguments
     * dim_num (Fixnum) : 0,1,...  0 is the fastest varying dimension.

     Return value
     *  a NetCDFDim object

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_vardimid)

---dims
     Returns an array of all the dimensions of the variable

     Arguments
     *  (none)

     Return value
     *  Array of NetCDFDim objects.

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_vardimid

---shape_ul0
     Returns the shape of the variable, but the length of the unlimited dimension is set to zero.
     Good to define another variable.

     Arguments
     *  (none)

     Return value
     *  Array. [length of 0th dim, length of 1st dim,.. ]

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_vardimid, nc_inq_unlimdim etc)

---shape_current
     Returns the current shape of the variable.

     Arguments
     *  (none)

     Return value
     *  Array. [length of 0th dim, length of 1st dim,.. ]

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_vardimid etc)

---each_att{ ... }
     Iterator regarding the global attributes of the variables.
     Ex.: {|i| print i.name,"\n"} prints names of all of them.

     Arguments
     * { ... }  : Block for the iterator. A "do end" block is the alternative.

     Return value
     *  self

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_natts, nc_inq_attname)

---dim_names
     Returns the names of all the dimensions of the variable

     Arguments
     *  (none)

     Return value
     *  Array of String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_varndims, nc_inq_vardimid, nc_inq_dimname)

---att_names
     Returns the names of all the attributes of the variable

     Arguments
     *  (none)

     Return value
     *  Array of String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (dependent on nc_inq_natts, nc_inq_attname)

---name
     Returns the name of the variable
 
     Arguments
     *  (none)

     Return value
     *  String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_varname

---name=(variable_newname)
     Rename the variable

     Arguments
      * variable_newname (String) : new name

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_rename_var

---ndims
     Number of dimensions of the variable (which is rank of the variable).

     Arguments
     *  (none)

     Return value
     *  Integer

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_varndims

---rank
     Aliased to ndims

---ntype
     Aliased to vartype

---vartype
     Inquires the data value type of the variable

     Arguments
     *  (none)

     Return value
     *  String ("char","byte","sint","int","sfloat", or "float")

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_vartype

---typecode
     Inquires the data type of the variable (returns a typecode of NArray)

     Arguments
     *  (none)

     Return value
     *  a Fixnum (NArray:BYTE, NArray:SINT, NArray:LINT, NArray:SFLOAT, NArray:SFLOAT, NArray:DFLOAT)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_vartype

---natts
     Returns the number of the attributes of the variable

     Arguments
     *  (none)

     Return value
     *  Integer

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_varnatts

---file
     Inquires the file that the variable is in

     Arguments
     *  (none)

     Return value
     *  a NetCDF object

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (none)

---att(attribute_name)
     Returns the attribute specified by its name

     Arguments
     * attribute_name (String) : Name of the attribute

     Return value
     *  a NetCDFAtt object if the attribute exists; nil if not

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (nc_inq_attid is used for inquiry)

---put_att(attribute_name, value, atttype=nil)
     Sets an attribute

     Arguments
     * attribute_name (String) : name of the attribute
     * value (Numeric, String, Array of Numeric, or NArray) : value of the attribute
     * atttype (nil or String) : data type of the attribute value.
       nil lets it automatically determined from the value.
       "char" (="string"), "byte", "sint", "int", "sfloat", or "float"
       specifies the type explicitly (1,1,2,4,4,8 bytes, respectively)

     Return value
     *  a NetCDFAtt object

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_put_att_<type>

---put(value, option=nil)
     Aliased to ((<simple_put>))

---simple_put(value, option=nil)
     Set the values of the variable

     Arguments
     * value : value to set (Numeric, Array of Numeric (1D only), or 
       NArray (possibly multi-D)). If it is a Numeric or length==1, the value
       is set uniformly.
     * option (Hash) : Optional argument to limit the portion of the
       variable to output values. If omitted, the whole variable is
       subject to the output. This argument accepts a Hash whose keys
       contain either "index" or a combination of "start","end", and
       "stride". The value of "index" points the index of a scalar
       portion of the variable. The other case is used to designate a
       regularly ordered subset, where "start" and "end" specifies
       bounds in each dimension and "stride" specifies intervals in
       it. As in Array "start", "end", and "index" can take negative
       values to specify index backward from the end. However,
       "stride" has to be positive, so reversing the array must be
       done afterwards if you like.

       Example: If the variable is 2D:

       {"start"=>[2,5],"end"=>[6,-1],"stride"=>[2,4]} -- Specifies a 
       subset made as follows: the 1st dimension from the element 2
       to the element 6 (note that the count starts with 0, so that
       the element 2 is the 3rd one) with an interval of 2; 
       the 2nd dimension from the element 6 to the last element
       (designated by -1) with an interval of 5.

       {"index"=>[0,0]}: Scalar of the fist element

       {"index"=>[0,-2]}: Scalar from the 1st element of with
       respect to the 1st dimension and the 2nd element from the last
       with respect to the 2nd dimension


     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_put_var_<type>, nc_put_vars_<type>, nc_put_var1_<type>

---pack(na)
     Pack a NArray (etc) using the attributes scale_factor and/or add_offset of self.

     If scale_factor and/or add_offset is defined, returns
     (na-add_offset)/scale_factor. Returns na if not.

     Arguments
     * na : a numeric array to pack (NArray, NArrayMiss, or Array)

     Return value
     *  a NArray or NArrayMiss

---scaled_put(value, option=nil)
     Same as ((<simple_put>)) but interprets the attributes scale_factor and/or add_offset using ((<pack>)).

     See the document for ((<simple_put>)) for arguments etc.

---get(option=nil)
     Aliased to ((<simple_get>)).

---simple_get(option=nil)
     Returns values of the variable

     Arguments
     * option (Hash) : Optional argument to limit the portion of the
       variable to get values. Its usage is the same as in the method
       put.

     Return value
     *  an NArray object

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_get_var_<type>, nc_get_vars_<type>, nc_get_var1_<type>

---unpack(na)
     Unpack a NArray (etc) using the attributes scale_factor and/or add_offset of self.

     If scale_factor and/or add_offset is defined, returns
     na * scale_factor + add_offset. Returns na if not.
     Type conversion is made by the coercing -- for example
     if na is sint and scale_factor and add_offset is sfloat,
     return value is sfloat. The type of the return value can be specified
     explicitly with ((<NetCDFVar.unpack_type=>)).

     Arguments
     * na : a numeric array to unpack (NArray, or NArrayMiss)

     Return value
     *  a NArray or NArrayMiss

---scaled_get(option=nil)
     Same as ((<simple_get>)) but interprets the attributes scale_factor and/or add_offset using ((<unpack>)).

     See the document for ((<simple_get>)) for arguments etc.

---[]
     Same as NetCDFVar#get but a subset is specified as in the method [] of NArray. 

     In addition to the subset specifications supported by NArray, 
     ranges with steps are supported, which is specified
     like {0..-1, 3}, i.e., a 1-element Hash with the key and value 
     representing the range (Range) and the step (Integer), respectively.
     Unlike NArray, 1-dimensional indexing of multi-dimensional
     variables is not support.

---[]=
     Same as NetCDFVar#put but a subset is specified as in the method []= of NArray. 

     In addition to the subset specifications supported by NArray, 
     ranges with steps are supported, which is specified
     like {0..-1, 3}, i.e., a 1-element Hash with the key and value 
     representing the range (Range) and the step (Integer), respectively.
     Unlike NArray, 1-dimensional indexing of multi-dimensional
     variables is not support.

===Instance Methods added by requiring "numru/netcdf_miss"

---get_with_miss(option=nil)
     Same as ((<get>)) but interprets data missing.

     Data missing is specified by the standard attributes valid_range,
     (valid_min and/or valid_max), or missing_value, with the precedence being 
     this order. Unlike the
     recommendation in the NetCDF User's guide, missing_value is
     interpreted if present. If missing_value and valid_* present
     simultaneously, missing_value must be outside the valid range.
     Otherwise, exception is raised.

     If data missing is specified as stated above, this method returns a NArrayMiss.
     If not, it returns a NArray. Thus, you can use this whether
     data missing is defined or not.     

     Arguments
     * See ((<get>)).

     Return value
     * an NArrayMiss (if data missing is specified) or an NArray
       (if data missing is NOT specified)

     Possible exception in addition to NetcdfError.
     * missing_value is in the valid range (see above).

     Corresponding (dependent) function(s) in the C library of NetCDF
     * See ((<get>)). This method is written in Ruby.

     EXAMPLE
     * The following is an example to replace ((<get>)) with ((<get_with_miss>)).
       It will also make ((<[]>)) interpret data missing, 
       since it calls (({get})) internally.

         file = NetCDF.open('hogehoge.nc')
         var = file.var('var')
         def var.get(*args); get_with_miss(*args); end
         p var.get       # --> interprets data missing if defined
         p var[0..-1,0]  # --> interprets data missing if defined (assumed 2D)

---get_with_miss_and_scaling(option=nil)
     Same as ((<get_with_miss>)) but handles data scaling too using ((<unpack>)).

     Missing data handling using valid_* / missing_value is applied 
     basically to packed data, which is consistent with most
     conventions. However, it is applied to unpacked data 
     if and only if the type of valid_* / missing_value is not the same as
     the packed data and is the samed as the unpacked data.
     This treatment can handle all conventions.

     EXAMPLE
     * See above. The same thing applies.

---put_with_miss(value, option=nil)
     Same as ((<put>)) but interprets data missing.

     If (({value})) is an NArray, the methods behaves as ((<put>)).
     Data missing in (({value})) is interpreted if it is an NArrayMiss
     and data missing is specified by attributes in (({self})) 
     (see ((<get_with_miss>)) ).
     Namely, the data which are "invalid" in the (({value})) is replaced
     with a missing value when written in the file.
     (missing_value or _FillValue or a value outside
     the valid range). No check is made whether "valid" values in the 
     NArrayMiss is within the valid range of (({self})).

     Arguments
     * value : NArrayMiss or what is allowed in ((<put>)).
     * option (Hash) : See ((<put>)).

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     * See ((<put>)). This method is written in Ruby.

     EXAMPLE
     * The following is an example to replace ((<put>)) with ((<put_with_miss>)).
       It will also make ((<[]=>)) interpret data missing, 
       since it calls (({put})) internally.

         file = NetCDF.open('hogehoge.nc')
         var = file.var('var')
         def var.put(*args); put_with_miss(*args); end
         var.put = narray_miss      # --> interprets data missing if defined
         var[0..-1,0] = narray_miss # --> interprets data missing if defined (assumed 2D)

---put_with_miss_and_scaling(value, option=nil)
     Same as ((<put_with_miss>)) but handles data scaling too using ((<pack>)).

     Missing data handling using valid_* / missing_value is applied 
     basically to packed data, which is consistent with most
     conventions. However, it is applied to unpacked data 
     if and only if the type of valid_* / missing_value is not the same as
     the packed data and is the samed as the unpacked data.
     This treatment can handle all conventions.

     EXAMPLE
     * See above. The same thing applies.

---------------------------------------------

=class NetCDFAtt
===Class Methods

===Instance Methods
---name
     Returns the name of the attribute

     Arguments
     *  (none)

     Return value
     *  String

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  (none)

---name=(attribute_newname)
     Rename the attribute

     Arguments
     * attribute_newname (String) : New name

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_rename_att

---copy(var_or_file)
     Copies an attribute to a variable or a file. If file, becomes an global attribute

     Arguments
     * var_or_file (NetCDFVar or NetCDF)

     Return value
     *  Resultant new attribute (NetCDFAtt)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_copy_att

---delete
     Delete an attribute

     Arguments
     *  (none)

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_del_att

---put(value, atttype=nil)
     Sets the value of the attribute

     Arguments
     * value (Numeric, String, Array of Numeric, or NArray) : value of the attribute
     * atttype (nil or String) : data type of the attribute value.
       nil lets it automatically determined from the value.
       "char" (="string"), "byte", "sint", "int", "sfloat", or "float"
       specifies the type explicitly (1,1,2,4,4,8 bytes, respectively)

     Return value
     *  nil

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_put_att_<type>

---get
     Returns the values of the attribute

     Arguments
     *  (none)

     Return value
     *  String or an NArray object (NOTE: even a scalar is returned as
        an NArray of length 1)
     
     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_get_att_<type>

---atttype
     Inquires the type of attribute values

     Arguments
     *  (none)

     Return value
     *  "char","byte","sint","int","sfloat","float"

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_atttype

---atttype
     Inquires the type of attribute values (returns a NArray typecode)

     Arguments
     *  (none)

     Return value
     *  a Fixnum (NArray:BYTE, NArray:SINT, NArray:LINT, NArray:SFLOAT, NArray:SFLOAT, NArray:DFLOAT)

     Corresponding (dependent) function(s) in the C library of NetCDF
     *  nc_inq_atttype

=end
