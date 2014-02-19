require 'omniauth'
require 'savon'

module OmniAuth
    module Strategies
        class TencentOA
            include OmniAuth::Strategy
            option :fields, [:login_name, :password]
            option :uid_field, :login_name

            attr_accessor :auth_info


            def request_phase
                #redirect client.auth_code.authorize_url({:redirect_uri => callback_url}.merge(options.authorize_params))
                form = OmniAuth::Form.new(:title => "Tencent Outlook Account", :url => callback_path)
                form.text_field "Rtx name", 'login_name'
                form.password_field "Outlook password", 'password'
                form.button "Sign In"
                form.to_response
            end

            uid do
                request.params['login_name']
            end

            info do
                { 
                    :name  => @auth_info[:chinese_name].to_s,
                    :email => @auth_info[:login_name] + '@tencent.com',
                    :uid   => @auth_info[:login_name].to_s
                }
            end

            credentials do
                {:token => @auth_info[:token].to_s, :secret => @auth_info[:key].to_s}
            end

            extra do 
                {
                    :raw_info   => @auth_info
                }
            end



            def callback_phase
                url = 'http://10.6.12.14/services/passportservice.asmx?WSDL'
                username = request['login_name'].to_s
                password = request['password'].to_s

                client = Savon.client(wsdl:url)
                begin
                    response = client.call(:authenticate_via_ldap, message:{:loginName => username, :password => password})
                rescue Exception => e
                    fail!(:auth_failed, e) 
                end 
                @auth_info = response.body[:authenticate_via_ldap_response][:authenticate_via_ldap_result]
                log :info, @auth_info.to_s
                super
            end
        end
    end
end
OmniAuth.config.add_camelization 'tencentoa', 'TencentOA'
