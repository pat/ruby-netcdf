require "numru/netcdf"
require "narray_miss"

module NumRu

  class NetCDFVar

    def get_with_miss(*args)
      __interpret_missing_params if !defined?(@missval)
      data = simple_get(*args)
      if @vmin || @vmax
	if @vmin
	  mask = (data >= @vmin) 
	  mask = mask.and(data <= @vmax) if @vmax
	else
	  mask = (data <= @vmax)
	end
	data = NArrayMiss.to_nam(data, mask)
      elsif @missval	# only missing_value is present.
	mask = (data.ne(@missval)) 
	data = NArrayMiss.to_nam(data, mask)
      end
      data
    end

    def get_with_miss_and_scaling(*args)
      __interpret_missing_params if !defined?(@missval)
      data = simple_get(*args)
      if @vmin || @vmax
	if @vmin
	  mask = (data >= @vmin) 
	  mask = mask.and(data <= @vmax) if @vmax
	else
	  mask = (data <= @vmax)
	end
	data = NArrayMiss.to_nam(data, mask)
      elsif @missval	# only missing_value is present.
	mask = (data.ne(@missval))
	data = NArrayMiss.to_nam(data, mask)
      end
      data = unpack( data )
      data
    end

    def put_with_miss(data, *args)
      if data.is_a?( NArrayMiss )
	__interpret_missing_params if !defined?(@missval)
	if @missval
	  simple_put(data.to_na(@missval), *args)
	else
	  simple_put(data.to_na, *args)
	end
      else
	simple_put(data, *args)
      end
    end

    def put_with_miss_and_scaling(data, *args)
      if data.is_a?( NArrayMiss )
	__interpret_missing_params if !defined?(@missval)
	if @missval
	  data = pack( data )
	  data = data.to_na(@missval)
	else
	  data = pack( data )
	  data = data.to_na
	end
	simple_put(data, *args)
      else
	scaled_put(data, *args)
      end
    end

    ######### private ##########

    def __interpret_missing_params
      # Interprets the specification of missing data,
      # either by valid_range, (valid_min and/or valid_max), or missing_value.
      # (unlike the NetCDF User's guide (NUG), missing_value is interpreted, 
      # but valid_* has a higher precedence.)
      # Always sets @missval whether missing_value is defined or not,
      # since it will be used as a fill value for data missing.
      #
      @vmin = att('valid_min')
      @vmin = @vmin.get if @vmin  # kept in a NArray(size==1) to consv type
      @vmax = att('valid_max')
      @vmax = @vmax.get if @vmax  # kept in a NArray(size==1) to consv type
      vrange = att('valid_range')
      vrange = vrange.get if vrange
      if vrange
	vrange.sort!
	@vmin = vrange[0..0]        # kept in... (same)
	@vmax = vrange[-1..-1]      # kept in... (same)
      end
      @missval = att('missing_value') || att('_FillValue')
      @missval = @missval.get if @missval # kept in... (same)

      sf = att('scale_factor')
      ao = att('add_offset')
      if ( sf || ao )
        ## Both NUG & CF conventions requires to specify the valid
        ## range with respect to the external (i.e. packed) values. 
        ## However, some conventions require specification
        ## with respect to unpacked values. The following
        ## is to support such cases as well:
        thres_tp = [ self.typecode, NArray::LINT ].max
        @missval = pack(@missval) if @missval && @missval.typecode > thres_tp
        @vmin = pack(@vmin) if @vmin && @vmin.typecode > thres_tp
        @vmax = pack(@vmax) if @vmax && @vmax.typecode > thres_tp
      end

      if @missval
        if @vmin && @vmax 
	  if @vmin[0] <= @missval[0] && @missval[0] <= @vmax[0]
            warn "WARNING: missing_value #{@missval[0]} is in the valid range #{@vmin[0]}..#{@vmax[0]} --> will be ignored (#{__FILE__}:#{__LINE__})"
          end
        else
          if @vmin && @missval[0] >= @vmin[0]
            warn "WARNING: missing_value #{@missval[0]} >= valid min #{@vmin[0]} --> will be ignored (#{__FILE__}:#{__LINE__})"
          elsif @vmax && @missval[0] <= @vmax[0]
            warn "WARNING: missing_value #{@missval[0]} <= valid min #{@vmin[0]} --> will be ignored (#{__FILE__}:#{__LINE__})"
          end
        end
      else
        realtc = NArray::SFLOAT
        if @vmin
          if @vmin[0] >= 0
            @missval = ( @vmin.typecode>=realtc ? 0.99*@vmin : @vmin-1 )
          else
            @missval = ( @vmin.typecode>=realtc ? 1.01*@vmin : @vmin-1 )
          end
        elsif @vmax
          if @vmax[0] >= 0
            @missval = ( @vmax.typecode>=realtc ? 1.01*@vmax : @vmax+1 )
          else
            @missval = ( @vmax.typecode>=realtc ? 0.99*@vmax : @vmax+1 )
          end
        end
      end

    end

    private :__interpret_missing_params

  end

end

if $0 == __FILE__
  include NumRu

  filename = "tmp.nc"
  print "creating ",filename,"...\n"
  file=NetCDF.create(filename)
  nx = 10
  dimx = file.def_dim("x",nx)
  xf = file.def_var("xf","sfloat",[dimx])
  xfn = file.def_var("xfn","sfloat",[dimx])
  xf.put_att("valid_range",[-1e12,1e12])
  f = 10 ** (2*NArray.sfloat(nx).indgen!)
  xr = file.def_var("xr","sint",[dimx])
  xr.put_att("valid_max",[0.5])
  xr.put_att("scale_factor",1e-4)
  xr.put_att("add_offset",0.5)
  xr2 = file.def_var("xr2","sint",[dimx])
  xr2.put_att("valid_max",NArray.sint(1).fill!(1000))
  xr2.put_att("scale_factor",1e-4)
  xr2.put_att("add_offset",0.5)
  r = NArray.sfloat(nx).indgen!/nx
  file.enddef
  xf.put(f)
  xfn.put(f)
  xr.scaled_put(r)
  file.close

  file = NetCDF.open(filename,'r+')
  xf = file.var('xf')
  xfn = file.var('xfn')
  p "f0"
  xf.get.each{|v| print "#{v} "} ; print "\n"
  p( 'f1', nam = xf.get_with_miss )
  def xf.get(*args); get_with_miss(*args); end
  p( 'f12',  xf[2..-3].to_na )
  p( 'fn10', xfn.get_with_miss )
  p( 'fn11', xfn.get_with_miss_and_scaling )
  nam.invalidation([0,1])
  p 'f2', nam
  xf.put_with_miss(nam)
  p( 'f3', xf.get_with_miss )
  xr = file.var('xr')
  p "r0"
  xr.simple_get.each{|v| print "#{v} "} ; print "\n"
  p( 'r1', xr.get_with_miss_and_scaling )
  def xr.get(*args); get_with_miss_and_scaling(*args); end
  def xr.put(*args); put_with_miss_and_scaling(*args); end
  #xr[0..3] = xr[0..3]*10
  p( 'r2', xr.get_with_miss_and_scaling )
  p 'r',r
  xr2.put_with_miss_and_scaling(r)
  p 'xr2',xr2.get_with_miss_and_scaling
  file.close
  print "** ncdump tmp.nc **\n", `ncdump tmp.nc`
end
