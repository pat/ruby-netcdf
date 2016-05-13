# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'version'

Gem::Specification.new do |spec|
  spec.name          = "netcdf-nmatrix"
  spec.version       = Ruby::Netcdf::VERSION
  spec.authors           = ["Takeshi Horinouchi", "Tsuyoshi Koshiro",\
    "Shigenori Otsuka", "Seiya Nishizawa", "T Sakakima", "Edmund Highcock"]
  spec.email            = ['edmundhighcock@users.sourceforge.net']

  #if spec.respond_to?(:metadata)
  #  spec.metadata['allowed_push_host'] = "TODO: Set to 'http://mygemserver.com' to prevent pushes to rubygems.org, or delete to allow pushes to any server."
  #end

  spec.summary          = %q{Ruby interface to NetCDF which uses NMatrix}
  spec.description      = %q{A Ruby interface for reading and writing NetCDF files. NetCDF-NMatrix is a rewrite of the previous ruby-netcdf gem which uses NMatrix instead of NArray, and so is compatible with SciRuby.}

  spec.homepage         = 'https://github.com/edmundhighcock/ruby-netcdf'
  spec.licenses         = ["GFD Dennou Club"]

  spec.files         = `git ls-files -z`.split("\x0")
  spec.test_files    = spec.files.grep(%r{^(test|demo)/})
  #spec.bindir        = "exe"
  #spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.required_ruby_version = Gem::Requirement.new(">= 1.6")
  spec.add_runtime_dependency(%q<nmatrix>, [">= 0.2"])

  spec.extensions << "extconf.rb"
end
