#pragma once

#include "preheader.h"
#include "transport.h"
#include "message.h"
#include "option.h"
#include "server_register.h"

namespace ucorf
{
    struct Session
    {
        SessId sess;
        ITransportServer *transport;
        IHeaderPtr header;
    };

    class IService;
    class ServerImpl
    {
    public:
        typedef boost::function<IMessage*(std::string const&, std::string const&)> MessageFactory;

        ServerImpl();
        ~ServerImpl();

        ServerImpl& SetOption(boost::shared_ptr<Option> opt);

        bool RegisterTo(std::string const& url);

        bool RegisterService(boost::shared_ptr<IService> service);

        void RemoveService(std::string const& service_name);

        boost_ec Listen(std::string const& url);

        /// --------------------------- extend method ---------------------------
    public:
        ServerImpl& BindTransport(std::unique_ptr<ITransportServer> && transport);
        ServerImpl& SetHeaderFactory(HeaderFactory const& head_factory);
        ServerImpl& SetRegister(boost::shared_ptr<IServerRegister> reg);
        /// ---------------------------------------------------------------------

    private:
        void OnConnected(ITransportServer *tp, SessId sess_id);
        void OnDisconnected(ITransportServer *tp, SessId sess_id, boost_ec const& ec);
        size_t OnReceiveData(ITransportServer *tp, SessId sess_id, const char* data, size_t bytes);

        bool DispatchMsg(Session & sess, const char* data, size_t bytes);

    private:
        typedef std::map<std::string, boost::shared_ptr<IService>> ServiceMap;
        typedef std::list<std::unique_ptr<ITransportServer>> TransportList;

        ServiceMap services_;
        boost::shared_ptr<Option> opt_;
        boost::shared_ptr<IServerRegister> register_;
        HeaderFactory head_factory_;
        TransportList transports_;
    };

} //namespace ucorf
