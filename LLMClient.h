#ifndef LLMCLIENT_H
#define LLMCLIENT_H

#include <string>

struct LLMResult {
    bool success;
    std::string meaning;
    std::string example;
    std::string raw;   // 调试时保留原始返回
};

class LLMClient {
    private:
    std::string modelName;
    std::string baseUrl;

    public:
    LLMClient(const std::string& model = "qwen3.5:4b",
              const std::string& url = "http://localhost:11434");

    LLMResult enrichWord(const std::string& word) const;
};

#endif