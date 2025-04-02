# 🐮 banchoo - 반추: 기록을 되새기는 LLM 노트 서버

`banchoo`는 사용자의 자율적인 기록을 중심에 두고, 그 기록에 대해 LLM이 사후적으로 주석을 달고 요약하고 분류하는 **지능형 노트 서버**입니다.

기존 노트 앱처럼 작성 도중에 보조하는 것이 아닌, **"작성 이후"**에 AI가 개입하여 기록을 해석하고, 되새기고, 의미를 정리해줍니다. 마치 반추동물이 음식을 되새김질하듯이, banchoo는 당신의 생각과 흔적을 다시 꺼내어 더 깊이 있는 인사이트를 제공합니다.

---

## ✨ 핵심 기능

- 📥 노트 저장: 사용자의 원본 기록을 그대로 저장
- 🧠 요약: LLM을 이용해 노트를 간결하게 요약
- 🏷️ 분류: 노트 내용을 자동으로 태깅하거나 분류 (예정)
- 🔎 자연어 검색: "3월에 회의한 내용 뭐였지?"처럼 자유로운 검색 (예정)
- 📆 시기별 요약: 특정 날짜 범위에 작성된 노트들을 LLM이 통합 요약 (예정)

---

## 🧩 기술 스택

- Language: **C++17**
- Web Framework: [**Crow**](https://github.com/CrowCpp/Crow)
- JSON 처리: [**nlohmann/json**](https://github.com/nlohmann/json)
- LLM 연동: 직접 로컬 모델 실행 (예: `llama.cpp`, `qwen`, `mistral`)
- 빌드: **CMake**

---

## 🛠️ 빠른 시작

```bash
# 프로젝트 클론
$ git clone https://github.com/yourname/banchoo.git
$ cd banchoo

# third_party 헤더 다운로드
$ curl -L https://raw.githubusercontent.com/CrowCpp/Crow/v1.0/include/crow_all.h -o third_party/crow_all.h
$ curl -L https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -o third_party/nlohmann/json.hpp

# 빌드
$ mkdir build && cd build
$ cmake ..
$ make

# 실행
$ ./llm_note_server
```

---

## 🌐 API 예시

### `POST /notes`

```json
{
  "content": "오늘 회의에서 새로운 전략을 논의함."
}
```

### `GET /notes/1`

```json
{
  "id": 1,
  "content": "오늘 회의에서 새로운 전략을 논의함."
}
```

### `GET /notes/1/summary`

```json
{
  "id": 1,
  "summary": "[LLM Summary] 오늘 회의에서..."
}
```

---

## 📌 철학

> 기록은 인간의 몫, 해석은 AI의 몫.

banchoo는 당신이 자유롭게 쓴 메모에 대해, 어떤 방향이나 형식을 강요하지 않고,
그 의미를 사후적으로 되새기며 정리해주는 **AI 기반 회고 노트**입니다.

기록을 쓰는 순간이 아닌, **다시 읽을 순간**을 위해 존재하는 노트 시스템.

---

## 📖 향후 계획

- [ ] 노트 타입 구분 (Memo / Task / Event)
- [ ] 태그 및 검색 기능
- [ ] LLM 모델 연동 (llama.cpp 등)
- [ ] 요약 스타일 설정 (짧게 / 자세히)
- [ ] SQLite 또는 파일 기반 저장소 연결

---

## 📛 네임스페이스 및 구조

모든 주요 코드는 `banchoo` 네임스페이스 아래에 작성됩니다.

```cpp
namespace banchoo {
    class NoteHandler { ... };
}
```

---

## 🧠 만든 이유
>
> 흔적은 남는다. 그리고 그 흔적은 언젠가 반추될 수 있다.

이 프로젝트는 생각과 삶의 흐름을 **AI가 해석 가능한 정보**로 정리할 수 있다는 믿음에서 출발했습니다. 쓰는 건 인간의 감성, 정리하는 건 AI의 분석력.

---

## © 2025 banchoo project – 기록을 되새기다
