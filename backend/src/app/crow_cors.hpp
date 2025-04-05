/*
 * Copyright (c) 2024 Lee Sangwon
 * This file is part of the Banchoo Project.
 * Licensed under the MIT License.
 */
#pragma once
#include <crow_all.h>

struct Cors
{
    struct context
    {
    };

    void before_handle(crow::request &req, crow::response &res, context &)
    {
        // OPTIONS preflight 처리
        if (req.method == "OPTIONS"_method)
        {
            res.code = 204;
            res.end();
        }
    }

    void after_handle(crow::request &, crow::response &res, context &)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods",
                       "GET, POST, PUT, DELETE, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};
