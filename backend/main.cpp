#include "crow.h" // Use crow.h instead of crow_all.h
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <iostream>
#include <string>

// Namespace aliases for convenience
using json = nlohmann::json;
using namespace std;

// Configuration Constants
const string OLLAMA_API_URL = "http://localhost:11434/v1/models/your-model-name/completions"; // Replace 'your-model-name' with the actual model
const string DB_CONN = "dbname=llmchat user=youruser password=yourpassword host=localhost"; // Update with your DB credentials

// Function to call Ollama's API
string callLLM(const string& prompt, double temperature, int max_tokens) {
    json requestBody;
    requestBody["prompt"] = prompt;
    requestBody["temperature"] = temperature;
    requestBody["max_tokens"] = max_tokens;

    // If Ollama requires additional parameters, include them here
    // For example, "top_p", "frequency_penalty", etc.

    // Make the POST request to Ollama's API
    cpr::Response r = cpr::Post(
        cpr::Url{OLLAMA_API_URL},
        cpr::Header{
            {"Content-Type", "application/json"}
            // {"Authorization", "Bearer YOUR_OLLAMA_API_KEY"} // Uncomment if authentication is required
        },
        cpr::Body{requestBody.dump()}
    );

    if (r.status_code == 200) {
        json response = json::parse(r.text);
        // Adjust the response parsing based on Ollama's API response structure
        // Assuming the response has a field "text" containing the completion
        if (response.contains("choices") && response["choices"].is_array() && !response["choices"].empty()) {
            return response["choices"][0]["text"].get<string>();
        } else if (response.contains("text")) {
            return response["text"].get<string>();
        } else {
            cerr << "Unexpected response format from Ollama." << endl;
            return "Error: Unexpected response format from LLM.";
        }
    } else {
        cerr << "Error calling Ollama API: " << r.status_code << " " << r.text << endl;
        return "Error: Unable to get response from LLM.";
    }
}

// Function to save message to the database
void saveMessageToDB(const string& type, const string& content) {
    try {
        pqxx::connection C(DB_CONN);
        pqxx::work W(C);
        W.exec("INSERT INTO conversation (type, content) VALUES (" +
               W.quote(type) + ", " + W.quote(content) + ");");
        W.commit();
    } catch (const exception &e) {
        cerr << "Database error: " << e.what() << endl;
    }
}

// Function to retrieve conversation history from the database
json getConversationHistory() {
    json history = json::array();
    try {
        pqxx::connection C(DB_CONN);
        pqxx::nontransaction N(C);
        pqxx::result R = N.exec("SELECT type, content FROM conversation ORDER BY id ASC;");
        for(auto row : R) {
            json message;
            message["type"] = row["type"].as<string>();
            message["content"] = row["content"].as<string>();
            history.push_back(message);
        }
    } catch (const exception &e) {
        cerr << "Database error: " << e.what() << endl;
    }
    return history;
}

int main()
{
    crow::SimpleApp app;

    // WebSocket route
    CROW_WEBSOCKET_ROUTE(app, "/ws")
    .onopen([](crow::websocket::connection& conn) {
        cout << "WebSocket connection opened" << endl;
    })
    .onmessage([](crow::websocket::connection& conn, const string& data, bool is_binary) {
        json received;
        try {
            received = json::parse(data);
        } catch (const json::parse_error& e) {
            cerr << "JSON parse error: " << e.what() << endl;
            return;
        }

        if(received.contains("type") && received["type"] == "user") {
            if (!received.contains("content") || !received["content"].is_string()) {
                cerr << "Invalid message format: 'content' missing or not a string." << endl;
                return;
            }

            string userMessage = received["content"];
            saveMessageToDB("user", userMessage);

            // Default settings; ideally, retrieve per-user settings
            double temperature = 0.7;
            int max_tokens = 150;

            // Call LLM
            string aiResponse = callLLM(userMessage, temperature, max_tokens);
            saveMessageToDB("ai", aiResponse);

            // Prepare and send response
            json response;
            response["type"] = "ai";
            response["content"] = aiResponse;
            conn.send_text(response.dump());
        }
        else if(received.contains("type") && received["type"] == "settings") {
            // Handle settings update if needed
            // For simplicity, settings are not stored per user in this example
            // You can extend this to handle user-specific settings
            cout << "Settings updated: " << received["content"] << endl;
            // Optionally, parse and apply settings here
        }
        else {
            cerr << "Unknown message type received." << endl;
        }
    })
    .onclose([](crow::websocket::connection& conn, const string& reason){
        cout << "WebSocket connection closed: " << reason << endl;
    });

    // API route to get conversation history
    CROW_ROUTE(app, "/api/history").methods("GET"_method)([](){
        json res;
        res["history"] = getConversationHistory();
        return crow::response{res.dump()};
    });

    // API route to save entire conversation
    CROW_ROUTE(app, "/api/save-conversation").methods("POST"_method)([](){
        // In this simple implementation, messages are already saved in real-time
        // You can implement additional logic if needed
        json res;
        res["status"] = "Conversation saved successfully.";
        return crow::response{res.dump()};
    });

    // API route to share conversation
    CROW_ROUTE(app, "/api/share-conversation").methods("POST"_method)([](){
        // Generate a unique conversation ID
        // For simplicity, using a static ID; in production, use UUIDs or similar
        string conversationId = "12345"; // Replace with actual ID generation logic

        json res;
        res["conversationId"] = conversationId;
        return crow::response{res.dump()};
    });

    // Route to display shared conversation
    CROW_ROUTE(app, "/share/<string>").methods("GET"_method)([](const string& conversationId){
        // Fetch and display the shared conversation based on the conversationId
        // For simplicity, returning a placeholder response
        // Implement actual retrieval based on conversationId
        json res;
        res["message"] = "This is a shared conversation with ID: " + conversationId;
        return crow::response{res.dump()};
    });

    // Serve frontend static files
    CROW_ROUTE(app, "/<path>")
    .methods("GET"_method)
    ([&app](const crow::request& req, crow::response& res, std::string path) {
        std::ifstream ifs("frontend/" + path, std::ios::in | std::ios::binary);
        if (ifs) {
            std::ostringstream ss;
            ss << ifs.rdbuf();
            res.write(ss.str());
            res.end();
        } else {
            res.code = 404;
            res.write("File not found");
            res.end();
        }
    });

    // Serve index.html for root path
    CROW_ROUTE(app, "/").methods("GET"_method)
    ([&app](const crow::request& req, crow::response& res){
        std::ifstream ifs("frontend/index.html", std::ios::in | std::ios::binary);
        if (ifs) {
            std::ostringstream ss;
            ss << ifs.rdbuf();
            res.write(ss.str());
            res.end();
        } else {
            res.code = 404;
            res.write("File not found");
            res.end();
        }
    });

    app.port(8080).multithreaded().run();
}
