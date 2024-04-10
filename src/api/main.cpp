#include "iostream"
#include "vector"
#include "utility"

#include "string"
#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")([](){
        return "Working fine...";
    });

    std::vector<std::pair<std::string, std::string>> blogs; // Vector to store blogs

    // Handler for POST request to add a blog
    CROW_ROUTE(app, "/api/blogs").methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid body");
        std::string title, content;
        try {
            title = body["title"].s();
            content = body["content"].s();
        } catch (const std::runtime_error &err) {
            return crow::response(400, "Invalid body");
        }

        // Add the new blog to the vector
        blogs.push_back(std::make_pair(title, content));

        return crow::response(200, "Blog added");
    });

    // Handler for GET request to retrieve all blogs
    CROW_ROUTE(app, "/api/blogs")
    ([&]()
    {
        std::vector<crow::json::wvalue> blog_data;
        for (const auto& blog : blogs)
        {
            blog_data.push_back(crow::json::wvalue{
                {"title", blog.first},
                {"content", blog.second}
            });
        }

        return crow::json::wvalue{{"data", blog_data}};
    });

    app.port(3000).multithreaded().run(); // Start the server
}
