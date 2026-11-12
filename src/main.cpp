#include <iostream>
#include <SFML/Graphics.hpp>
#include "crow.h"

int main() 

{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]()
        {
            return "server is runnning!";
        });


    CROW_WEBSOCKET_ROUTE(app, "/ws")

        .onopen([](crow::websocket::connection& conn)
            {
                std::cout << "Client Connected" << std::endl;
            })

        .onmessage([](crow::websocket::connection& conn, const std::string& message, bool isBinary)

            {
                if (isBinary) return;

                std::cout << "Received" << message << std::endl;

                conn.send_text("Server received: " + message);
            })

        .onclose([](crow::websocket::connection& conn, const std::string& reason)
            {
                std::cout << "client disconnected" << std::endl;
            });

    
         
        
    std::cout << "Starting the server...." << std::endl;

    app.port(18080).run();

    return 0; 

}