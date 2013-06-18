No changes beyond a single require statement in extconf.rb to ensure the gem compiles within MRI 2.0.0.

```ruby
gem 'ruby-netcdf', '~> 0.6.6.1',
  :git    => 'git://github.com/pat/ruby-netcdf.git',
  :branch => 'master',
  :ref    => '6b514b8ec4'
```
