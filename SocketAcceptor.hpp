#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "SocketConnection.hpp"

class SocketAcceptor {
public:
    bool initialize(boost::asio::io_context& ioCtx,
                    uint16_t port);

	bool accept(std::shared_ptr<SocketConnection>& outConn);

    bool stop();

private:
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
};

