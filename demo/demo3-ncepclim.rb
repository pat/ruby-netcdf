# Plot global climatological temperature distribution
# from the NCEP reanalysis data.
# The data is downloaded if not found and the users wants.

######################################
### local functions  ###
def nearest_index(na,val)
   # returns the element of na nearest to val
   # na is assumed to be 1d and monotonic

   if(na[0] > na[-1]) then
      reversed = true
      na = na[-1..0]
   else
      reversed = false
   end

   w = (na.lt(val)).where
   idx = [ (w.length==0 ? 0 : w.max), na.length-2 ].min
   if ( na[idx+1]-val < val-na[idx] ) then
      idx = idx+1
   end

   if reversed then
      na = na[-1..0]
      idx = na.length-1-idx
   end

   idx
end
#####################################
### main part ###
require "numru/dcl"
require "numru/netcdf"
include NumRu

filename = "air.mon.ltm.nc"

# < download if not found and the users wants >

if !(Dir.glob(filename)[0]) then
   # file not found ==> download by ftp if the user wants
   host = "ftp.cdc.noaa.gov"
   path = "/Datasets/ncep.reanalysis.derived/pressure/"+filename
   print "\n*** question ***\n",
         "  File "+filename+" is not in the current directory.\n",
         "  Would you like to download (ftp) it from "+host+"?\n",
         "  (y, n)> "
   ans = gets
   if ans =~ /^y/ then
      print "  What is your email address? (needed for anonymous ftp) > "
      email = gets.chop!
      require "net/ftp"
      print "  connecting...\n"
      ftp = Net::FTP.open(host, "anonymous", email, nil) 
      size = ftp.size(path)
      print "  Size of the file is #{(size/1000)} kb. Would you really like to download?\n  (y, n)> "
      ans = gets
      if ans =~ /^y/ then
	 print "  now downloading...\n"
	 ftp.getbinaryfile(path, filename)
      else
	 print "  exit\n"
	 exit
      end
   else
      print "  exit\n"
      exit
   end
end

# < open the file and read axes >

file = NetCDF.open(filename)
var = file.var("air")      # temperature

lon = file.var("lon")
lat = file.var("lat")
level = file.var("level")
time = file.var("time")     # in hours
t = (time.get/720).round + 1     # --> in months
axes = [ lon, lat, level, time ]
axvals = [ lon.get, lat.get, level.get, t ]
axunits = [ lon.att("units").get.gsub("_",""), 
            lat.att("units").get.gsub("_",""), 
            level.att("units").get.gsub("_",""),
            "months" ]
# < graphics >

#DCL.sglset('lbuff',false)
DCL.swlset('lwait',false)
DCL.gropn(1)

first = true
while true do
   begin

      ## / select a 2D slice /

      if (first) then
	 print <<-EOS
	 ** select a slice **
	 List desired grid numbers of lonigutede, latitude, level, time
	 (set only two of them)

	 Example: 
	   , , 850, 1
		   -- horizontal slice at 850hPa and January
	   135, , , 2
		   -- vertical slice at 135E and Feburary
	 EOS
	 DCL.grfrm
      else
	 DCL.grfrm
	 print "Input next slice (C-d to quit)\n"
      end
      print "> "
      slice = gets.chop!.split(",")
      if slice.length!=4 then
	 raise("Slice must be 4 comma-split numerics")
      end
      slice.collect!{|e|    # "collect!" replaces elements
	 if e =~ /^ *$/ then
	    nil
	 else
	    e.to_f
	 end
      }

      iax = [] 
      start=[] ; last=[]
      slice.each_index{|i|
	 if slice[i] == nil then
	    iax.push(i)
	    start.push(0) ; last.push(-1)   # from the beginning to the end
	 else
	    idx = nearest_index( axvals[i], slice[i] )
	    start.push( idx ) ; last.push( idx )
	 end
      }

      if iax.length != 2 then
	 raise("Specify a 2D slice")
      else
	 x = axvals[iax[0]]
	 iax[0]==2 ? xr=[x.max, x.min] : xr=[x.min, x.max]
	 xttl = axes[iax[0]].att("long_name").get
	 xunits = axunits[iax[0]]
	 y = axvals[iax[1]]
	 iax[1]==2 ? yr=[y.max, y.min] : yr=[y.min, y.max]
	 yttl = axes[iax[1]].att("long_name").get
	 yunits = axunits[iax[1]]
      end

      ## / read the slice and plot /

      v = var.get("start"=>start, "end"=>last)
      shp=v.shape; shp.delete(1); v.reshape!(*shp)  # delete dims of length==1

      #Fig.inclpoint(x, y)
      DCL.grswnd( xr[0], xr[1], yr[0], yr[1] )
      DCL.grsvpt(0.2,0.9,0.2,0.9)
      DCL.grstrf
      DCL.ussttl(xttl," ",yttl," ")
      DCL.usdaxs
      DCL.uwsgxa(x)
      DCL.uwsgya(y)
      DCL.uelset("ltone",true)
      DCL.uetone(v)
      DCL.udcntz(v)

      first = false
   rescue
      print "*Error*  ", $!,"\n"     # show the error message in ($!)
   end
end

DCL.grcls
