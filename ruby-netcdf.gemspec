# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'version'

Gem::Specification.new do |spec|
  spec.name          = "ruby-netcdf"
  spec.version       = Ruby::Netcdf::VERSION
  spec.authors           = ["Takeshi Horinouchi", "Tsuyoshi Koshiro",\
    "Shigenori Otsuka", "Seiya Nishizawa", "T Sakakima"]
  spec.email            = ['eriko@gfd-dennou.org']

  #if spec.respond_to?(:metadata)
  #  spec.metadata['allowed_push_host'] = "TODO: Set to 'http://mygemserver.com' to prevent pushes to rubygems.org, or delete to allow pushes to any server."
  #end

  spec.summary          = %q{Ruby interface to NetCDF}
  spec.description      = %q{RubyNetCDF is the Ruby interface to the NetCDF library built on the NArray library, which is an efficient multi-dimensional numeric array class for Ruby. This version works with Ruby2.0.}

  spec.homepage         = 'http://www.gfd-dennou.org/arch/ruby/products/ruby-netcdf/'
  spec.licenses         = ["GFD Dennou Club"]

  spec.files         = `git ls-files -z`.split("\x0")
  spec.test_files    = spec.files.grep(%r{^(test|demo)/})
  #spec.bindir        = "exe"
  #spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.required_ruby_version = Gem::Requirement.new(">= 1.6")
  spec.add_runtime_dependency(%q<narray>, [">= 0"])
  spec.add_runtime_dependency(%q<narray_miss>, [">= 0"])

  spec.extensions << "extconf.rb"
end
