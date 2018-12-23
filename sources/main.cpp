// Copyright 2018 Avsyankaa Avsyankaa@gmail.com

#include <client.hpp>

boost::asio::ip::tcp::endpoint ep(
    boost::asio::ip::address::from_string("127.0.0.1"), 8001);
void run_client(const std::string & client_name)
{
    talk_to_svr client(client_name);
    try
    {
        client.connect(ep);
        client.loop();
    }
    catch(boost::system::system_error & err)
    {
        std::cout << "client terminated " << std::endl;
    }
}

int main() {
    std::thread thread1(run_client, "Asya");
    std::thread thread2(run_client, "Roma");
    thread1.join();
    thread2.join();
    return 0;
}
