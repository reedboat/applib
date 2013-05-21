# -*- encoding: utf-8 -*-
require File.expand_path('../lib/omniauth-tencentoa/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["reedboat"]
  gem.email         = ["zhqm03@gmail.com"]
  gem.description   = %q{Tencent OA authentication}
  gem.summary       = %q{Tencent OA authentication}
  gem.homepage      = ""

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "omniauth-tencentoa"
  gem.require_paths = ["lib"]
  gem.version       = OmniAuth::TencentOA::VERSION
end
