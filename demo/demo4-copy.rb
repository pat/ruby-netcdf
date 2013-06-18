=begin

=demo4-copy.rb

Make a copy of a NetCDF file

==Usage

% ruby demo4-copy.rb filename_from filename_to

=end

def usage
"\n\nUSAGE:\n% ruby #{$0} filename_from filename_to\n"
end

require "numru/netcdf"
include NumRu
raise usage if ARGV.length != 2
filename_from, filename_to = ARGV
from = NetCDF.open(filename_from)
to = NetCDF.create(filename_to)
from.each_dim{|dim| to.def_dim( dim.name, dim.length_ul0 )}
from.each_att{|att| to.put_att( att.name, att.get )}    ## global attributes
from.each_var{|var|
  newvar = to.def_var( var.name, var.ntype, var.dim_names )
  var.each_att{|att| newvar.put_att( att.name, att.get )}
}
to.enddef
from.each_var{|var| to.var(var.name).put(var.get)}
to.close

