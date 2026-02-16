#include "SmtpParser.hpp"

#include <algorithm>
static std::string toUpper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){
                       return std::toupper(c);
                   });
    return str;
}

SmtpCommand SmtpParser::parse(const std::string& input)
{
    SmtpCommand cmd;
    std::string upper = toUpper(input);

    if (upper.rfind("HELO", 0) == 0)
    {
        cmd.type = SmtpCommandType::HELO;
        cmd.argument = input.substr(5);
    }
    else if (upper.rfind("EHLO", 0) == 0)
    {
        cmd.type = SmtpCommandType::EHLO;
        cmd.argument = input.substr(5);
    }
    else if (upper.rfind("MAIL FROM:", 0) == 0)
    {
        cmd.type = SmtpCommandType::MAIL;
        cmd.argument = input.substr(10);
    }
    else if (upper.rfind("RCPT TO:", 0) == 0)
    {
        cmd.type = SmtpCommandType::RCPT;
        cmd.argument = input.substr(8);
    }
    else if (upper.rfind("DATA", 0) == 0)
    {
        cmd.type = SmtpCommandType::DATA;
    }
    else if (upper.rfind("QUIT", 0) == 0)
    {
        cmd.type = SmtpCommandType::QUIT;
    }

    return cmd;
}

