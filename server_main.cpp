#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "SocketAcceptor.hpp"
#include "SmtpSession.hpp"
#include "ThreadPool.hpp"

int main()
{
    try
    {
        boost::asio::io_context ioContext;

        SocketAcceptor acceptor;

        if (!acceptor.initialize(ioContext, 25000))
        {
            std::cout << "Failed to initialize acceptor\n";
            return 1;
        }

        ThreadPool pool(4);

        std::cout << "SMTP Server running on port 25000\n";

        while (true)
        {
			std::shared_ptr<SocketConnection> connection;

            if (!acceptor.accept(connection))
                continue;

            pool.enqueue(
                [conn = std::move(connection)]() mutable
                {
                    SmtpSession session;

                    conn->send("220 SMTP Server Ready\r\n");

                    std::string input;

                    while (conn->receive(input))
                    {
                        std::string response =
                            session.processCommand(input);

                        if (!response.empty())
                            conn->send(response);

                        if (response.find("221") != std::string::npos)
                            break;
                    }

                    conn->close();
                });
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Server error: "
                  << e.what() << "\n";
    }

    return 0;
}

