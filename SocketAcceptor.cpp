#include "SocketAcceptor.hpp"

bool SocketAcceptor::initialize(
    boost::asio::io_context& ioCtx,
    uint16_t port)
{
    try {
        m_acceptor = std::make_unique<
            boost::asio::ip::tcp::acceptor>(
            ioCtx,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(),
                port)
        );
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SocketAcceptor::accept(
    std::shared_ptr<SocketConnection>& outConn)
{
    if (!m_acceptor)
        return false;

    boost::asio::ip::tcp::socket socket(
        m_acceptor->get_executor());

    boost::system::error_code ec;
    m_acceptor->accept(socket, ec);

    if (ec)
        return false;

	outConn = std::make_shared<SocketConnection>();
    return outConn->initialize(std::move(socket));
}

bool SocketAcceptor::stop()
{
    if (!m_acceptor)
        return false;

    boost::system::error_code ec;
    m_acceptor->close(ec);
    return true;
}

