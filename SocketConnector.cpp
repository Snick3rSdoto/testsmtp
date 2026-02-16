#include "SocketConnector.hpp"

bool SocketConnector::initialize(
    boost::asio::io_context& ioCtx)
{
    m_ioCtx = &ioCtx;
    return true;
}

bool SocketConnector::connect(
    const std::string& host,
    uint16_t port,
    std::unique_ptr<SocketConnection>& outConn)
{
    if (!m_ioCtx)
        return false;

    try {
        boost::asio::ip::tcp::resolver resolver(*m_ioCtx);
        auto endpoints =
            resolver.resolve(host,
                             std::to_string(port));

        boost::asio::ip::tcp::socket socket(*m_ioCtx);

        boost::asio::connect(socket, endpoints);

        outConn = std::make_unique<SocketConnection>();
        return outConn->initialize(std::move(socket));
    }
    catch (...)
    {
        return false;
    }
}

