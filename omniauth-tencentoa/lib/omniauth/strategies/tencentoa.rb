require 'omniauth'
require 'savon'

module OmniAuth
    module Strategies
        class TencentOA
            include OmniAuth::Strategy
            option :fields, [:name, :password]
            option :uid_field, :name

            attr_accessor :auth_info


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
                request.params['name']
            end

            info do
                { :name => @auth_info[:name], :nickname => @auth_info[:chinese_name],   :password => request.params['password'] }
            end

            credentials do
                {:token => @auth_info[:token]}
            end

            extra do 
                {
                    :raw_info   => @auth_info
                }
            end



            def callback_phase
                url = 'http://10.6.12.14/services/passportservice.asmx?WSDL'
                client = Savon.client(wsdl:url)
                response = client.call(:AuthenticateViaLdap, message:{loginName => username, password => password})
                self.auth_info = response.body
                log :info, self.auth_info.to_yaml
                super
            end
        end
    end
end
OmniAuth.config.add_camelization 'tencentoa', 'TencentOA'
