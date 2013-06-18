require "rubygems" unless defined?(Gem)
require "mkmf"

hoge = Gem::GemPathSearcher.new.find("narray")
narray_fullname = hoge.full_name
narray_installpath = hoge.installation_path
narray_include = "#{narray_installpath}/gems/#{narray_fullname}/"
narray_lib = "#{narray_installpath}/gems/#{narray_fullname}/"

dir_config('narray',narray_include,narray_lib)
dir_config('netcdf','/usr/local')

if ( ! ( have_header("narray.h") && have_header("narray_config.h") ) ) then
print <<EOS
** configure error **  
   Header narray.h or narray_config.h is not found. If you have these files in 
   /narraydir/include, try the following:

   % ruby extconf.rb --with-narray-include=/narraydir/include

EOS
   exit(-1)
end

if xsystem("ncdap-config --libs")
  libncdods = "nc-dap"
  cflags = `ncdap-config --cflags`.gsub(/\n/, " ")
  libs = `ncdap-config --libs`.gsub(/\n/, " ")
  prefix_dods = `ncdap-config --prefix`.gsub(/\n/, "")
elsif xsystem("opendap-config --libs")
  libncdods = "nc-dods"
  cflags = `opendap-config --cflags`.gsub(/\n/, " ")
  libs = `opendap-config --libs-nc`.gsub(/\n/, " ")
  prefix_dods = `opendap-config --prefix`.gsub(/\n/, "")
end

if File.directory?("/usr/include/netcdf")  #-- for Vine linux
  cflags = "-I/usr/include/netcdf"
  libs = "-L/usr/lib"
  $CFLAGS += ' ' + cflags
  $LOCAL_LIBS += ' ' + libs
end
 
if (enable_config('opendap',true) && ( xsystem("opendap-config --libs") ||
                                       xsystem("ncdap-config --libs") ) )

  dir_config(libncdods,prefix_dods)

  if (!have_library(libncdods))
    print <<-EOS
    ** ERROR ** Library not found: nc-dods (OPeNDAP/DODS-enabled NetCDF lib)
    Install it, or run extconf.rb with option --disable-opendap.
                                              ^^^^^^^^^^^^^^^^^
    EOS
    exit(-1)
  else
    print <<-EOS
    ** Message **  Compiling with OPeNDAP/DODS-enabled NetCDF library.
 
    This is because the command opendap-config is found in your system.
    If you want to use the ordinary (non-DODS) version of NetCDF,
    run extconf.rb with option --disable-opendap.
                               ^^^^^^^^^^^^^^^^^
    EOS
  end

  $CFLAGS += ' '+cflags
  $LOCAL_LIBS += ' ' + libs

  # non portable treatments: should be improved (by Horinouchi)
  CONFIG['LDSHARED'].sub!(/gcc/,'g++')
  $LIBS.sub!(/-lc\s/,'') ; $LIBS.sub!(/-lc$/,'')
  print <<-EOS
    ** Warning **  non-portable treatments are made, 
    which was sucessfull redhat linux 9:
     * gcc was replaced with g++ in CONFIG['LDSHARED']
     * -lc library was removed if in $LIBS 

  EOS
  #  p '@@@'
  #  ary = []
  #  CONFIG.each{|k,v| ary.push([k,v])}
  #  ary.sort.each{|x| p x}
else
  if xsystem("nc-config --libs") # for NetCDF 4
    cflags = `nc-config --cflags`.gsub(/\n/, " ")
    libs = `nc-config --libs`.gsub(/\n/, " ")
    prefix_nc = `nc-config --prefix`.gsub(/\n/, "")

    dir_config("netcdf",prefix_nc)
    $CFLAGS += ' ' + cflags
    $LOCAL_LIBS += ' ' + libs
  end
  if ( ! ( have_header("netcdf.h") && have_library("netcdf") ) )then
    print <<-EOS
    ** configure error **  
       Header netcdf.h or the compiled netcdf library is not found. 
       If you have the library installed under /netcdfdir (that is, netcdf.h is
       in /netcdfdir/include and the library in /netcdfdir/lib/),
       try the following:

       % ruby extconf.rb --with-netcdf-dir=/netcdfdir

       Alternatively, you can specify the two directory separately
       with --with-netcdf-include and --with-netcdf-lib.
    EOS
    exit(-1)
  end
end

if /cygwin|mingw/ =~ RUBY_PLATFORM
   have_library("narray") || raise("ERROR: narray library is not found")
end

create_makefile "numru/netcdfraw"

######  Modify Makefile: #######
File.rename("Makefile","Makefile.orig")
oldmkfl = File.open("Makefile.orig")
newmkfl = File.open("Makefile","w")
oldmkfl.each_line{ |line|
   case(line)
   when /^distclean:/
      newmkfl.puts(line)
      newmkfl.puts("\t\t@$(RM) *.nc demo/*.nc demo/*~ lib/*~ doc/*~ test/*.nc test/*~ Makefile.orig")
   when /^all:/
      newmkfl.puts(line)
      newmkfl.puts("")         
      newmkfl.puts("test: all")            # insert the "test" target
      newmkfl.puts("\t\t@cd test && ruby test.rb && echo 'test did not fail :-p (please ignore the warnings)' && cd ..") 
   when /lib\/netcdf/
      line = line.chomp! + "/"
      newmkfl.puts(line)
   else
      newmkfl.puts(line)
   end
}
newmkfl.close
