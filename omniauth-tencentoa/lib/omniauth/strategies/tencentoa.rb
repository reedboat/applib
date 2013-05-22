require 'omniauth'

module OmniAuth
    module Strategies
        class TencentOA
            include OmniAuth::Strategy
            option :fields, [:name, :password]
            option :uid_field, :name

            attr_accessor :access_token


            def request_phase
                #redirect client.auth_code.authorize_url({:redirect_uri => callback_url}.merge(options.authorize_params))
                form = OmniAuth::Form.new(:title => "User Info", :url => callback_path)
                options.fields.each do |field|
                    form.text_field field.to_s.capitalize.gsub("_", " "), field.to_s
                end
                form.button "Sign In"
                form.to_response
            end

            uid do
                log :info, request.params.to_yaml
                request.params['name']
            end

            info do
                { :name => request.params['name'],   :password => request.params['password'] }
            end

            credentials do
                {:token => 'oa-token'}
            end

            #def callback_phase
                #if request.params['password'] != request.params['name'] + '.qq'
                    #return !fail('username or password incorrect')
                #end
                #super
            #end
        end
    end
end
OmniAuth.config.add_camelization 'tencentoa', 'TencentOA'
