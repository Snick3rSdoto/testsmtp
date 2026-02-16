#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "SocketConnection.hpp"

class SocketConnector {
public:
    bool initialize(boost::asio::io_context& ioCtx);

    bool connect(const std::string& host,
                 uint16_t port,
                 std::unique_ptr<SocketConnection>& outConn);

private:
    boost::asio::io_context* m_ioCtx = nullptr;
};

