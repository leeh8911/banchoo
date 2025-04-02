/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */

#pragma once

namespace banchoo::app
{

class IApp
{
  public:
    virtual ~IApp() = default;

    // 앱 설정 (DI 등)
    virtual void configure() = 0;

    // 서버 실행
    virtual void run() = 0;
};
} // namespace banchoo::app