#include "LLMClient.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
static std::string extractJsonStringField(const std::string& text, const std::string& key);
static std::string unescapeJsonString(const std::string& s);
static std::string extractJsonStringField(const std::string& text, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t start = text.find(pattern);
    if (start == std::string::npos) {
        return "";
    }

    start += pattern.size();
    while (start < text.size() && (text[start] == ' ' || text[start] == '\t' || text[start] == '\r' || text[start] == '\n')) {
        ++start;
    }
    if (start >= text.size() || text[start] != ':') {
        return "";
    }
    ++start;
    while (start < text.size() && (text[start] == ' ' || text[start] == '\t' || text[start] == '\r' || text[start] == '\n')) {
        ++start;
    }
    if (start >= text.size() || text[start] != '"') {
        return "";
    }
    ++start;

    size_t end = start;
    bool escape = false;

    while (end < text.size()) {
        char c = text[end];

        if (escape) {
            escape = false;
        } else {
            if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                break;
            }
        }
        ++end;
    }

    if (end >= text.size()) {
        return "";
    }

    return text.substr(start, end - start);
}
static std::string unescapeJsonString(const std::string& s) {
    std::string result;
    bool escape = false;

    for (char c : s) {
        if (escape) {
            if (c == '"' || c == '\\' || c == '/') {
                result += c;
            } else if (c == 'n') {
                result += '\n';
            } else if (c == 't') {
                result += '\t';
            } else {
                result += c;
            }
            escape = false;
        } else {
            if (c == '\\') {
                escape = true;
            } else {
                result += c;
            }
        }
    }

    return result;
}
LLMClient::LLMClient(const std::string& model, const std::string& url)
    : modelName(model), baseUrl(url) {
}

LLMResult LLMClient::enrichWord(const std::string& word) const {
    LLMResult result;
    result.success = false;
    result.meaning = "";
    result.example = "";
    result.raw = "";

    std::string prompt =
        "For the English word " + word +
        ", return JSON only with keys meaning and example.";

    std::string requestFile = "D:\\tools\\ollama_request.json";
    std::string responseFile = "D:\\tools\\ollama_response.json";

    std::ofstream req(requestFile);
    if (!req) {
        return result;
    }

    req << "{\n";
    req << "  \"model\": \"" << modelName << "\",\n";
    req << "  \"prompt\": \"" << prompt << "\",\n";
    req << "  \"format\": \"json\",\n";
    req << "  \"think\": false,\n";
    req << "  \"stream\": false\n";
    req << "}\n";
    req.close();

    std::string command =
        "curl.exe -sS " + baseUrl + "/api/generate "
        "-H \"Content-Type: application/json\" "
        "-d @" + requestFile + " "
        "-o " + responseFile;

    int code = std::system(command.c_str());
    if (code != 0) {
        return result;
    }

    std::ifstream in(responseFile);
    if (!in) {
        return result;
    }

    std::ostringstream ss;
    ss << in.rdbuf();
    result.raw = ss.str();
    std::string inner = extractJsonStringField(result.raw, "response");
    if (inner.empty()) {
        inner = extractJsonStringField(result.raw, "thinking");
    }
    if (inner.empty()) {
        return result;
    }
    inner = unescapeJsonString(inner);
    result.meaning = extractJsonStringField(inner,"meaning");
    result.example = extractJsonStringField(inner,"example");
    if (!result.meaning.empty() && !result.example.empty()) {
        result.success = true;
    }

    return result;
}
