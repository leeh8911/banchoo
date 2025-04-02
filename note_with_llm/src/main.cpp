/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */                                                                                                                    \
#include "app/app_factory.hpp"

int main()
{
    auto app = banchoo::app::AppFactory::create("crow");

    app->configure(); // DI, 라우팅 등 구성
    app->run();       // 서버 실행

    return 0;
}
