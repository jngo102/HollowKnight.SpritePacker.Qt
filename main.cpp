#include "sprite_packer.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SpritePacker window;
    window.show();
    return app.exec();
}
