#include <QApplication>
#include "romanceclub.h"

int main(int argc, char *argv[])
{
    // Инициализация приложения
    QApplication a(argc, argv);

    // Установка стиля для более приятного отображения
    a.setStyle("Fusion");

    // Создание главного окна приложения
    RomanceClub window;

    // Настройка размера окна
    window.resize(800, 600);
    window.setMinimumSize(640, 480);

    // Загрузка истории и запуск первой сцены
    window.loadStory();
    window.start();

    // Отображение окна
    window.show();

    // Запуск основного цикла приложения
    return a.exec();
}
