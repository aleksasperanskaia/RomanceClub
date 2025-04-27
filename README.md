#Romance Club Visual Novel - README
Описание проекта
Визуальная новелла "Romance Club" создана с использованием фреймворка Qt (C++). Проект представляет собой интерактивную историю с ветвящимся сюжетом, где игрок принимает решения, влияющие на развитие событий.

Особенности
Ветвящийся сюжет с множеством вариантов развития

Поддержка фоновых изображений и персонажей

JSON-формат для хранения сценария

Простое добавление новых сцен и вариантов выбора

Требования
Qt 6.x

Компилятор C++ с поддержкой C++17

CMake 3.5+ или qmake

Установка и запуск
Сборка с qmake
Клонируйте репозиторий:

bash
git clone [URL репозитория]
cd RomanceClubv2
Соберите проект:

bash
qmake RomanceClubv2.pro
make
Запустите приложение:

bash
./RomanceClubv2
Запуск из Qt Creator
Откройте файл RomanceClubv2.pro в Qt Creator

Нажмите "Сборка" → "Собрать проект"

Запустите приложение (Ctrl+R)

Структура проекта
RomanceClubv2/
├── RomanceClubv2.pro       - Файл проекта qmake
├── main.cpp                - Точка входа в приложение
├── romanceclub.h           - Заголовочный файл главного окна
├── romanceclub.cpp         - Реализация главного окна
├── romanceclub.qrc         - Файл ресурсов Qt
├── story.json              - Сценарий новеллы в формате JSON
├── backgrounds/            - Фоновые изображения
│   ├── street.jpg
│   ├── room.jpg
│   ├── sea.jpg
│   ├── wedding.jpg
│   └── car.jpg
└── characters/             - Изображения персонажей
    ├── farima.jpg
    ├── attila.jpg
    ├── allahverdi.jpg
    ├── rauf.jpg
    └── leyla.jpg
Формат story.json
Сценарий описывается в JSON-формате:

json
{
    "scenes": [
        {
            "id": "start",
            "text": "Текст сцены...",
            "choices": [
                {
                    "text": "Вариант выбора 1",
                    "next_scene": "scene1"
                },
                {
                    "text": "Вариант выбора 2",
                    "next_scene": "scene2"
                }
            ]
        }
    ]
}
Добавление контента
Новые фоны: поместите изображения в папку backgrounds и добавьте в romanceclub.qrc

Новые персонажи: аналогично, в папку characters

Новые сцены: редактируйте story.json, соблюдая структуру
