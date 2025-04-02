/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#pragma once

#include <string>

#include <third_party/nlohmann/json.hpp>
namespace banchoo::app
{

class IApp
{
  public:
    virtual ~IApp() = default;

    // 앱 설정 (DI 등)
    virtual void configure(const nlohmann::json &config) = 0;

    // 서버 실행
    virtual void run() = 0;

    // 포트 설정
    void setPort(uint32_t port)
    {
        port_ = port;
    }
    uint32_t getPort() const
    {
        return port_;
    }

    // 바인딩 주소 설정
    void setBindAddr(const std::string &bindaddr)
    {
        bindaddr_ = bindaddr;
    }

    const std::string &getBindAddr() const
    {
        return bindaddr_;
    }

  private:
    uint32_t port_{0};
    std::string bindaddr_{};
};
} // namespace banchoo::app