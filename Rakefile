require 'rubygems/package_task'

NAME = 'ruby-netcdf-updated'
VER = '0.7.0'

PKG_FILES = FileList[
  '**',
  'lib/**/*',
  'demo/**/*',
  'doc/**/*',
  'test/**/*'
]

spec = Gem::Specification.new do |s|
  s.name             = NAME
  s.version          = VER
  s.authors           = ["Takeshi Horinouchi", "Tsuyoshi Koshiro",\
    "Shigenori Otsuka", "Seiya Nishizawa", "T Sakakima", "Edmund Highcock"]
  s.email            = ['edmundhighcock@users.sourceforge.net']
  s.homepage         = 'http://www.gfd-dennou.org/arch/ruby/products/ruby-netcdf/'
  s.licenses         = ["GFD Dennou Club"]
  s.platform         = Gem::Platform::RUBY
  s.summary          = %q{Ruby interface to NetCDF}
  s.description      = %q{RubyNetCDF is the Ruby interface to the NetCDF library built on the NArray library, which is an efficient multi-dimensional numeric array class for Ruby. This is an updated version that works with Ruby 2.0. Contact cannot be established with the original developers.}

  s.files            = PKG_FILES.to_a
  s.require_paths    = ['lib']
  s.test_files = Dir.glob("test/*")
  s.has_rdoc         = true
  s.required_ruby_version = Gem::Requirement.new(">= 1.6")
  s.add_runtime_dependency(%q<narray>, [">= 0"])
  s.add_runtime_dependency(%q<narray_miss>, [">= 0"])
  #s.extra_rdoc_files = ['README']

  s.extensions << "extconf.rb"
end

Gem::PackageTask.new(spec) do |pkg|
  #pkg.gem_spec = spec
  pkg.need_tar = true
end

task :clean do
	system "rm -rf Makefile *.o *.so mkmf.log test/test.nc *.gem"
end
