require 'narray'
require 'numru/netcdfraw'

if NArray.const_defined?(:SUPPORT_BIGMEM) && NArray::SUPPORT_BIGMEM
  unless NumRu::NetCDF::SUPPORT_BIGMEM
    raise "Ruby-NetCDF was compiled with NArray with big memory supoort " +
          "(NArray-bigmem). However the NArray loaded is not NArray-bigmem."
  end
else
  if NumRu::NetCDF::SUPPORT_BIGMEM
    raise "Ruby-NetCDF was compiled with NArray without big memory support. " +
          "However the NArray loaded is with the support (NArray-bigmem)."
  end
end

module NumRu
  class NetCDF

    Max_Try = 100

    NCVERSION = NetCDF.libvers

    if NCVERSION[0..0] >= "4"
      @@nc4 = true
    else
      @@nc4 = false
    end
    def NetCDF.nc4?
      @@nc4
    end

    @@cr_format = 0

    def NetCDF.creation_format=(cmode)
      raise("This method is available only for NetCDF >= 4") unless @@nc4
      case cmode
      when  0, nil, NC_CLASSIC_MODEL, /^CLASSIC$/i  # classic netcdf ver 3 fmt
        @@cr_format = 0
      when NC_64BIT_OFFSET, /^64BIT_OFFSET$/i
        @@cr_format = NC_64BIT_OFFSET
      when NC_NETCDF4, /^NETCDF4$/i
        @@cr_format = NC_NETCDF4
      when ( NC_NETCDF4 | NC_CLASSIC_MODEL), /^NETCDF4_CLASSIC$/i
        # NetCDF4 but disabling new data models
        @@cr_format = NC_NETCDF4 | NC_CLASSIC_MODEL
      else
        raise ArgumentError, "Unsupported creation mode: #{cmod.to_s}"
      end
    end

    def NetCDF.creation_format
      raise("This method is available only for NetCDF >= 4") unless @@nc4
      case @@cr_format
      when 0
        "TRADITIONAL"
      when NC_64BIT_OFFSET
        "64BIT_OFFSET"
      when NC_NETCDF4
        "NETCDF4"
      when NC_NETCDF4 | NC_CLASSIC_MODEL
        "NETCDF4_CLASSIC"
      end
    end

    def NetCDF.open(filename,mode="r",share=false)
       call_create=false   # false-> nc_open; true->nc_create
       case(mode)
       when "r","rb"                          # read only
	  mode=NC_NOWRITE
       when "w","w+","wb","w+b"               # overwrite if exits
          call_create=true
	  mode=NC_CLOBBER
       when "a","a+","r+","ab","a+b","r+b"    # append if exits
	  if( File.exists?(filename) )
	     mode=NC_WRITE
	  else
	     call_create=true   #(nonexsitent --> create)
	     mode=NC_CLOBBER
	  end
       else
	  raise NetcdfError, "Mode #{mode} is not supported"
       end
       case(share)
       when false
	  share=0
       when true
	  share=NC_SHARE
       else
	  raise NetcdfError, "We can't use the sharing mode you typed"
       end
       omode = mode | share
       if(!call_create)
	  nc_open(filename,omode)
       else
	  nc_create(filename,omode)
       end
    end
    
    class << NetCDF
      alias new open
    end
    
    
    def NetCDF.create(filename,noclobber=false,share=false)
      case(noclobber)
      when false
	noclobber=NC_CLOBBER
      when true
	noclobber=NC_NOCLOBBER
      else
	raise NetcdfError,"noclobber (2nd argument) must be true or false"
      end
      case(share)
      when false
	share=0
      when true
	share=NC_SHARE
      else
	raise NetcdfError,"share (3rd argument) must be true or false"
      end
      
      cmode=noclobber | share | @@cr_format
      nc_create(filename,cmode)
    end
    
    class << NetCDF
       def clean_tmpfile(path)
	  proc {
	     print "removing ", path, "..." if $DEBUG
	     if File.exist?(path)
		File.unlink(path) 
	     end
	     print "done\n" if $DEBUG
	  }
       end
       protected :clean_tmpfile
    end

    def NetCDF.create_tmp(tmpdir=ENV['TMPDIR']||ENV['TMP']||ENV['TEMP']||'.', 
			  share=false)
       basename = 'temp'
       if $SAFE > 0 and tmpdir.tainted?
	  tmpdir = '.'
       end

       n = 0
       while true
	 begin
	   tmpname = sprintf('%s/%s%d_%d.nc', tmpdir, basename, $$, n)
	   unless File.exist?(tmpname)
	      netcdf = NetCDF.create(tmpname, true, share)
	      ObjectSpace.define_finalizer(netcdf, 
					   NetCDF.clean_tmpfile(tmpname))
	      break
	   end
	 rescue
	   raise NetcdfError, "cannot generate tempfile `%s'" % tmpname if n >= Max_Try
	 end
	 n += 1
       end
       netcdf
    end


    def put_att(attname,val,atttype=nil)
       put_attraw(attname,val,atttype)
    end
    
    def def_var_with_dim(name, vartype, shape_ul0, dimnames)
       # Same as def_var but defines dimensions first if needed.
       # Use zero in shape to define an unlimited dimension.
       if (shape_ul0.length != dimnames.length ) then
	  raise ArgumentError, 'lengths of shape and dimnames do not agree'
       end
       dims = []
       dimnames.each_index{ |i|
	  dim = self.dim( dimnames[i] )
	  if ( dim != nil ) then
	     # dim exists --> check the length
	     if (shape_ul0[i] != dim.length_ul0 ) then
		raise ArgumentError, "dimension length do not agree: #{i}th dim: "+\
		"#{shape_ul0[i]} and #{dim.length_ul0}"
	     end
	     dims.push(dim)
	  else
	     # dim does not exist --> define it
	     dims.push( def_dim( dimnames[i], shape_ul0[i] ) )
	  end
       }
       def_var(name, vartype, dims)
    end

    # Iterators:
    def each_dim
      num_dim=ndims()    
      for dimid in 0..num_dim-1
	obj_Dim=id2dim(dimid)
	 yield(obj_Dim)
      end
    end
    
    def each_var
      num_var=nvars()
      for varid in 0..num_var-1
	obj_Var=id2var(varid)
	yield(obj_Var)
      end
    end
    
    def each_att
      num_att=natts()
      for attnum in 0..num_att-1
	obj_Att=id2att(attnum)
	 yield(obj_Att)
      end
    end
    
    def dims( names=nil )   # return all if names==nil
       if names == nil
	  dims = (0..ndims()-1).collect{|dimid| id2dim(dimid)}
       else
	  raise TypeError, "names is not an array" if ! names.is_a?(Array)
	  dims = names.collect{|name| dim(name)}
	  raise ArgumentError, "One or more dimensions do not exist" if dims.include?(nil)
       end
       dims
    end

    def vars( names=nil )   # return all if names==nil
       if names == nil
	  vars = (0..nvars()-1).collect{ |varid| id2var(varid) }
       else
	  raise TypeError, "names is not an array" if ! names.is_a?(Array)
	  vars = names.collect{|name| var(name)}
	  raise ArgumentError, "One or more variables do not exist" if vars.include?(nil)
       end
       vars
    end

    def dim_names
      num_dim=ndims()
      names=[]
      for dimid in 0..num_dim-1
	obj_Dim=id2dim(dimid)    
	names=names+[obj_Dim.name]
      end
      return names
    end    
    
    def var_names
      num_var=nvars()
      names=[]
      for varid in 0..num_var-1
	obj_Var=id2var(varid)
	names=names+[obj_Var.name]
      end
      return names
    end
    
    def att_names
      num_att=natts()
      names=[]
      for attnum in 0..num_att-1
	obj_Att=id2att(attnum)    
	names=names+[obj_Att.name]
      end
      return names
    end
    
    def inspect
      "NetCDF:"+path
    end
    
  end
  
  class NetCDFVar
    
    class << NetCDFVar
       def new(file,varname,mode="r",share=false)
	  if(file.is_a?(String))
	     file = NetCDF.open(file,mode,share)
	  elsif(!file.is_a?(NetCDF))
	    raise TypeError, "1st arg must be a NetCDF (file object) or a String (path)"
	  end
	  file.var(varname)
       end

       alias open new
    end

    alias :rank :ndims

    def each_att
      num_att=natts()
      for attnum in 0..num_att-1
	obj_Att=id2att(attnum)
	yield(obj_Att)
      end
    end
    
    def dim_names
      ary = Array.new()
      dims.each{|dim| ary.push(dim.name)}
      ary
    end
    
   def att_names
     num_att=natts()
     names=[]
     for attnum in 0..num_att-1
       obj_Att=id2att(attnum)
       names=names+[obj_Att.name]
     end
     return names
   end
   
   def put_att(attname,val,atttype=nil)
      put_attraw(attname,val,atttype)
   end

   def shape_ul0
      sh = []
      dims.each{|d|
	 if d.unlimited? then
	    sh.push(0)
	 else
	    sh.push(d.length)
	 end
      }
      sh
   end
   
   def shape_current
      sh = []
      dims.each{|d|
	 sh.push(d.length)
      }
      sh
   end
   
   # The put and get methods in the NetCDFVar class

   def pack(na)
     sf = att('scale_factor')
     ao = att('add_offset')
     if ( sf == nil && ao == nil ) then
       na
     else
       na = NArray.to_na(na) if na.is_a?(Array)
       if sf
	 csf = sf.get
	 raise NetcdfError,"scale_factor is not a numeric" if csf.is_a?(String)
	 raise NetcdfError, "scale_factor is not unique" if csf.length != 1
	 raise NetcdfError, "zero scale_factor" if csf[0] == 0
       else
	 csf = nil
       end
       if ao
	 cao = ao.get
	 raise NetcdfError, "add_offset is not a numeric" if cao.is_a?(String)
	 raise NetcdfError, "add_offset is not unique" if cao.length != 1
       else
	 cao = nil
       end
       if csf and cao
	 packed = (na - cao) / csf
       elsif csf
	 packed = na / csf
       elsif cao
	 packed = na - cao
       end
       if self.typecode <= NArray::LINT
         packed = packed.round
       end
       packed
     end
   end
   
   def scaled_put(var,hash=nil)
     simple_put( pack(var), hash)
   end

   @@unpack_type = nil
   class << NetCDFVar
     def unpack_type
       @@unpack_type
     end
     def unpack_type=(na_type)
       if [NArray::BYTE, NArray::SINT, NArray::INT, 
	   NArray::SFLOAT, NArray::FLOAT, nil].include?(na_type)
	 @@unpack_type = na_type
       else
	 raise ArgumentError, "Arg must be one of NArray::BYTE, NArray::SINT, NArray::INT, NArray::SFLOAT, NArray::FLOAT"
       end
     end

   end

   def unpack(na)
     sf = att('scale_factor')
     ao = att('add_offset')
     if ( sf == nil && ao == nil ) then
       na
     else
       if sf
	 csf = sf.get
	 raise NetcdfError,"scale_factor is not a numeric" if csf.is_a?(String)
	 raise NetcdfError, "scale_factor is not unique" if csf.length != 1
	 raise NetcdfError, "zero scale_factor" if csf[0] == 0
       else
	 csf =nil
       end
       if ao
	 cao = ao.get
	 raise NetcdfError, "add_offset is not a numeric" if cao.is_a?(String)
	 raise NetcdfError, "add_offset is not unique" if cao.length != 1
       else
	 cao = nil
       end
       if csf and cao
	 una = na * csf + cao  # csf & cao are NArray -> coerced to their types
       elsif csf
	 una = na * csf
       elsif cao
	 una = na + cao
       end
       una = una.to_type(@@unpack_type) if @@unpack_type
       una
     end
   end

   def scaled_get(hash=nil)
     unpack( simple_get(hash) )
   end

   def simple_put(var,hash=nil)
     if hash==nil
       if self.vartype == "char"
	 put_var_char(var)
       elsif self.vartype == "byte"
	 put_var_byte(var)
       elsif self.vartype == "sint"
	 put_var_sint(var)
       elsif self.vartype == "int"
	 put_var_int(var)
       elsif self.vartype == "sfloat"
	 put_var_sfloat(var)
       elsif self.vartype == "float"
	 put_var_float(var)
       else 
	 raise NetcdfError,"variable type isn't supported in netCDF" 
       end
     elsif hash.key?("index")==true 
       if self.vartype == "char"
	 put_var1_char(var,hash["index"])
       elsif self.vartype=="byte"
	 put_var1_byte(var,hash["index"])
       elsif self.vartype=="sint"
	 put_var1_sint(var,hash["index"])
       elsif self.vartype == "int"
	 put_var1_int(var,hash["index"])
       elsif self.vartype == "sfloat"
	 put_var1_sfloat(var,hash["index"])
       elsif self.vartype == "float"
	 put_var1_float(var,hash["index"])
       else 
	 raise NetcdfError,"variable type isn't supported in netCDF"
       end
     elsif hash.key?("start")==true
       if hash.key?("end")==false && hash.key?("stride")==false
	 if self.vartype == "char"
	   put_vars_char(var,hash["start"],nil,nil)
	 elsif self.vartype=="byte"
	   put_vars_byte(var,hash["start"],nil,nil)
	 elsif self.vartype=="sint"
	   put_vars_sint(var,hash["start"],nil,nil)
	 elsif self.vartype=="int"
	   put_vars_int(var,hash["start"],nil,nil)
	 elsif self.vartype=="sfloat"
	   put_vars_sfloat(var,hash["start"],nil,nil)
	 elsif self.vartype=="float"
	   put_vars_float(var,hash["start"],nil,nil)
	 else
	   raise NetcdfError, "variable type isn't supported in netCDF"
	 end
       elsif hash.key?("end")==true && hash.key?("stride") == false
	 if self.vartype == "char"
	   put_vars_char(var,hash["start"],hash["end"],nil)
	 elsif self.vartype=="byte"
	   put_vars_byte(var,hash["start"],hash["end"],nil)
	 elsif self.vartype=="sint"
	   put_vars_sint(var,hash["start"],hash["end"],nil)
	 elsif self.vartype=="int"
	   put_vars_int(var,hash["start"],hash["end"],nil)
	 elsif self.vartype == "sfloat"
	   put_vars_sfloat(var,hash["start"],hash["end"],nil)
	 elsif self.vartype =="float"
	   put_vars_float(var,hash["start"],hash["end"],nil)
	 else
	   raise NetcdfError, "variable type isn't supported in netCDF"
	 end
       elsif hash.key?("end")==false && hash.key?("stride")==true
	 if self.vartype == "char"
	   put_vars_char(var,hash["start"],nil,hash["stride"])
	 elsif self.vartype=="byte"
	   put_vars_byte(var,hash["start"],nil,hash["stride"])
	 elsif self.vartype=="sint"
	   put_vars_sint(var,hash["start"],nil,hash["stride"])
	 elsif self.vartype=="int"
	   put_vars_int(var,hash["start"],nil,hash["stride"])
	 elsif self.vartype=="sfloat"
	   put_vars_sfloat(var,hash["start"],nil,hash["stride"])
	 elsif self.vartype=="float"
	   put_vars_float(var,hash["start"],nil,hash["stride"])
	 else
	   raise NetcdfError, "variable type isn't supported in netCDF"
	 end
       else hash.key?("end")==true && hash.key?("stride")==true
	 if self.vartype == "char"
	   put_vars_char(var,hash["start"],hash["end"],hash["stride"])
	 elsif self.vartype=="byte"
	   put_vars_byte(var,hash["start"],hash["end"],hash["stride"])
	 elsif self.vartype=="sint"
	   put_vars_sint(var,hash["start"],hash["end"],hash["stride"])
	 elsif self.vartype=="int"
	   put_vars_int(var,hash["start"],hash["end"],hash["stride"])
	 elsif self.vartype=="sfloat"
	   put_vars_sfloat(var,hash["start"],hash["end"],hash["stride"])
	 elsif self.vartype=="float"
	   put_vars_float(var,hash["start"],hash["end"],hash["stride"])
	 else
	   raise NetcdfError, "variable type isn't supported in netCDF"
	 end
       end
     else
       raise ArgumentError,"{'start'}=>[ARRAY] or {'index'}=>[ARRAY] is needed"
     end
   end

   alias put simple_put

   def simple_get(hash=nil)
     t_var = self.vartype
     if hash == nil
       if t_var == "char"
	 get_var_char
       elsif t_var == "byte"
	 get_var_byte
       elsif t_var == "sint"
	 get_var_sint
       elsif t_var == "int"
	 get_var_int
       elsif t_var == "sfloat"
	 get_var_sfloat
       elsif t_var == "float"
	 get_var_float
       else
	  raise NetcdfError, "variable type #{t_var} isn't supported in netCDF"
       end
     elsif hash.key?("index")==true
       ind = hash["index"]
       if t_var == "char"
	 get_var1_char(ind)
       elsif t_var == "byte"
	 get_var1_byte(ind)
       elsif t_var == "sint"
	 get_var1_sint(ind)
       elsif t_var == "int"
	 get_var1_int(ind)
       elsif t_var == "sfloat"
	 get_var1_sfloat(ind)
       elsif t_var == "float"
	 get_var1_float(ind)
       else
	 raise NetcdfError,"variable type #{t_var} isn't supported in netCDF"
       end
     elsif hash.key?("start")==true
       h_sta = hash["start"]
       h_end = hash["end"]     # can be nill
       h_str = hash["stride"]  # can be nill
       if NetCDF.nc4? && h_str && ((xstr=h_str[0]) != 1)
         # Tentative treatment for the very slow netcdf-4 reading with step.
         # Reading with step is generally slow with NetCDF 4, but it is
         # particularly so for the first dimension.
         # Ref: http://www.unidata.ucar.edu/mailing_lists/archives/netcdfgroup/2013/msg00311.html
         h_str[0] = 1 
         nc4remedy = true
       else
         nc4remedy = false
       end
       if t_var == "char"
         v = get_vars_char(h_sta,h_end,h_str)
       elsif t_var == "byte"
         v = get_vars_byte(h_sta,h_end,h_str)
       elsif t_var == "sint"
         v = get_vars_sint(h_sta,h_end,h_str)
       elsif t_var == "int"
         v = get_vars_int(h_sta,h_end,h_str)
       elsif t_var == "sfloat"
         v = get_vars_sfloat(h_sta,h_end,h_str)
       elsif t_var == "float"
         v = get_vars_float(h_sta,h_end,h_str)
       else
         raise NetcdfError, "variable type #{t_var} isn't supported in netCDF"
       end
       if nc4remedy
         idx = []
         (0...v.shape[0]).step(xstr){|k| idx.push(k)}
         v = v[idx,false]
       end
       v
     else
       raise ArgumentError,"{'start'}=>{ARRAY} or {'index'}=>{ARRAY} is needed"
     end
   end

   alias get simple_get

   def __rubber_expansion( args )
     if (id = args.index(false))  # substitution into id
       # false is incuded
       alen = args.length
       if args.rindex(false) != id
	 raise ArguemntError,"only one rubber dimension is permitted"
       elsif alen > rank+1
	 raise ArgumentError, "too many args"
       end
       ar = ( id!=0 ? args[0..id-1] : [] )
       args = ar + [true]*(rank-alen+1) + args[id+1..-1]
     elsif args.length == 0   # to support empty [], []=
       args = [true]*rank
     end
     args
   end
   private :__rubber_expansion

   def [](*a)
     if a.length == 0
       return self.get
     end
     a = __rubber_expansion(a)
     first = Array.new
     last = Array.new
     stride = Array.new
     set_stride = false
     a.each{|i|
       if(i.is_a?(Fixnum))
	 first.push(i)
	 last.push(i)
	 stride.push(1)
       elsif(i.is_a?(Range))
	 first.push(i.first)
	 last.push(i.exclude_end? ? i.last-1 : i.last)
	 stride.push(1)
       elsif(i.is_a?(Hash))
	 r = (i.to_a[0])[0]
         s = (i.to_a[0])[1]
         if ( !( r.is_a?(Range) ) || ! ( s.is_a?(Integer) ) )
	    raise TypeError, "Hash argument must be {a_Range, step}"
	 end
	 first.push(r.first) 
	 last.push(r.exclude_end? ? r.last-1 : r.last)
	 stride.push(s)
	 set_stride = true
       elsif(i.is_a?(TrueClass))
	 first.push(0)
	 last.push(-1)
	 stride.push(1)
       elsif( i.is_a?(Array) || i.is_a?(NArray))
	 a_new = a.dup
	 at = a.index(i)
	 i = NArray.to_na(i) if i.is_a?(Array)
	 for n in 0..i.length-1
	   a_new[at] = i[n]..i[n]
	   na_tmp = self[*a_new]
	   if n==0 then
	     k = at
	     if at > 0
	       a[0..at-1].each{|x| if x.is_a?(Fixnum) then k -= 1 end}
	     end
	     shape_tmp = na_tmp.shape
	     shape_tmp[k] = i.length
	     na = na_tmp.class.new(na_tmp.typecode,*shape_tmp)
	     index_tmp = Array.new(shape_tmp.length,true)
	   end
	   index_tmp[k] = n..n
	   na[*index_tmp] = na_tmp
	 end
	 return na
       else
	 raise TypeError, "argument must be Fixnum, Range, Hash, TrueClass, Array, or NArray"
       end
     }

     if(set_stride)
       na = self.get({"start"=>first, "end"=>last, "stride"=>stride})
     else
       na = self.get({"start"=>first, "end"=>last})
     end
     shape = na.shape
     (a.length-1).downto(0){ |i|
	 shape.delete_at(i) if a[i].is_a?(Fixnum)
      }
      na.reshape!( *shape )
     na
   end

   def []=(*a)
     val = a.pop
     a = __rubber_expansion(a)
     first = Array.new
     last = Array.new
     stride = Array.new
     set_stride = false
     a.each{|i|
       if(i.is_a?(Fixnum))
	 first.push(i)
	 last.push(i)
	 stride.push(1)
       elsif(i.is_a?(Range))
	 first.push(i.first)
	 last.push(i.exclude_end? ? i.last-1 : i.last)
	 stride.push(1)
       elsif(i.is_a?(Hash))
	 r = (i.to_a[0])[0]
         s = (i.to_a[0])[1]
         if ( !( r.is_a?(Range) ) || ! ( s.is_a?(Integer) ) )
	    raise ArgumentError, "Hash argument must be {first..last, step}"
	 end
	 first.push(r.first) 
	 last.push(r.exclude_end? ? r.last-1 : r.last)
	 stride.push(s)
	 set_stride = true
       elsif(i.is_a?(TrueClass))
	 first.push(0)
	 last.push(-1)
	 stride.push(1)
       elsif(i.is_a?(Array) || i.is_a?(NArray))
	 a_new = a.dup
	 at = a.index(i)
	 i = NArray.to_na(i) if i.is_a?(Array)
	 val = NArray.to_na(val) if val.is_a?(Array)
	 rank_of_subset = a.dup.delete_if{|v| v.is_a?(Fixnum)}.length
	 if val.rank != rank_of_subset
	   raise "rank of the rhs (#{val.rank}) is not equal to the rank "+
                 "of the subset specified by #{a.inspect} (#{rank_of_subset})"
	 end
	 k = at
	 a[0..at-1].each{|x| if x.is_a?(Fixnum) then k -= 1 end}
	 if i.length != val.shape[k]
	   raise "length of the #{k+1}-th dim of rhs is incorrect "+
                 "(#{i.length} for #{val.shape[k]})"
	 end
	 index_tmp = Array.new(val.rank,true) if !val.is_a?(Numeric) #==>Array-like
	 for n in 0..i.length-1
	   a_new[at] = i[n]..i[n]
	   if !val.is_a?(Numeric) then
	     index_tmp[k] = n..n
	     self[*a_new] = val[*index_tmp]
	   else
	     self[*a_new] = val
	   end
	 end
	 return self
       else
	 raise TypeError, "argument must be Fixnum, Range, Hash, TrueClass, Array, or NArray"
       end
     }

     if(set_stride)
	self.put(val, {"start"=>first, "end"=>last, "stride"=>stride})
     else
	self.put(val, {"start"=>first, "end"=>last})
     end
   end

   def inspect
     'NetCDFVar:'+file.path+'?var='+name
   end
   
 end
 
 class NetCDFAtt
   
   def put(val,atttype=nil)
      putraw(val,atttype)
   end
   
   def inspect
     'NetCDFAtt:'+name
   end
 end
 
 class NetCDFDim
   def inspect
     'NetCDFDim:'+name
   end

   def length_ul0
      if unlimited?
	 0
      else
	 length
      end
   end

 end
end
