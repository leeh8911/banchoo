// LLM 노트 서버 - C++ Crow 기반 템플릿
// 기능: 노트 저장, 요약 (LLM 로컬 모델 직접 로딩)

#include <unordered_map>
#include <string>
#include <mutex>
#include <cstdlib>
#include <iostream>

#include <third_party/crow_all.h>
#include <third_party/nlohmann/json.hpp>

using json = nlohmann::json;

// 메모 모델
struct Note {
    int id;
    std::string content;
};

// 메모 저장소 (간단한 인메모리)
std::unordered_map<int, Note> notes;
std::mutex notes_mutex;
int next_id = 1;

// LLM 요약 함수 (예: llama.cpp 로컬 실행 등)
std::string summarize_with_llm(const std::string& input) {
    // TODO: llama.cpp, mistral, qwen 등 로컬 모델 연동
    // 현재는 임시 요약 텍스트 리턴
    return "[LLM Summary] " + input.substr(0, 20) + "...";
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/notes").methods("POST"_method)([](const crow::request& req) {
        auto body = json::parse(req.body);
        std::lock_guard<std::mutex> lock(notes_mutex);

        Note note;
        note.id = next_id++;
        note.content = body["content"];
        notes[note.id] = note;

        json res = { {"id", note.id}, {"content", note.content} };
        return crow::response{res.dump()};
    });

    CROW_ROUTE(app, "/notes/<int>").methods("GET"_method)([](int id) {
        std::lock_guard<std::mutex> lock(notes_mutex);
        if (notes.find(id) == notes.end()) {
            return crow::response(404);
        }
        json res = { {"id", id}, {"content", notes[id].content} };
        return crow::response{res.dump()};
    });

    CROW_ROUTE(app, "/notes/<int>/summary").methods("GET"_method)([](int id) {
        std::lock_guard<std::mutex> lock(notes_mutex);
        if (notes.find(id) == notes.end()) {
            return crow::response(404);
        }
        std::string summary = summarize_with_llm(notes[id].content);
        json res = { {"id", id}, {"summary", summary} };
        return crow::response{res.dump()};
    });

    std::cout << "[INFO] LLM 노트 서버 실행 중: http://localhost:18080" << std::endl;
    app.port(18080).multithreaded().bindaddr("0.0.0.0").run();

}
