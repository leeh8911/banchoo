#include "app/crow_app.hpp"

int main()
{
    banchoo::app::CrowApp app;

    app.configure(); // DI, 라우팅 등 구성
    app.run();       // 서버 실행

    return 0;
}
