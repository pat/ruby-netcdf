require "numru/dcl"
require "numru/netcdf"
include NumRu
include NMath

## < create a sample netcdf file >
def write_file

   file = NetCDF.create("test.nc")
   nx, ny = 21, 21
   xdim = file.def_dim("x",nx)
   ydim = file.def_dim("y",ny)
   
   x = file.def_var("x","sfloat",["x"])
   y = file.def_var("y","sfloat",["y"])
   var = file.def_var("var","float",["x","y"])
   var.put_att("long_name","test variable")
   file.enddef

   vx =  NArray.float(nx).indgen! * (2*(Math::PI)*1.5/(nx-1))
   vy =  NArray.float(ny).indgen! * (2*(Math::PI)*1.0/(ny-1))
   x.put( vx )
   y.put( vy )

   sx = sin( vx )
   sy = sin( vy )
   z = NArray.float(nx,ny)
   for j in 0..ny-1
      z[true,j] = sx * sy[j] + 0.00001
   end

   var.put(z)

   file.close
   print `ncdump -h test.nc`
end

## < read the file and plot >
def draw_graph

   file = NetCDF.open("test.nc")
   vx = file.var("x")
   vy = file.var("y")
   vz = file.var("var")
   x = vx.get
   y = vy.get
   z = vz.get
   file.close

   #DCL.swlset('ldump',1)
   DCL.gropn(1)
   DCL.grfrm
   DCL.usspnt(x, y)
   DCL.uspfit
   DCL.usdaxs
   DCL.sglset("lsoftf",false)
   DCL.uegtlb(z, -20)  # set the number of levels
   DCL.uelset("ltone",true)
   DCL.uetone(z)
   DCL.udcntz(z)
   DCL.grcls
end

###(main)###
write_file
draw_graph
###(main)###
