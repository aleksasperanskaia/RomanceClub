#include "romanceclub.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QPixmap>
#include <QStackedWidget>
#include <QDir>
#include <QScrollArea>
#include <QApplication>

RomanceClub::RomanceClub(QWidget *parent) : QMainWindow(parent)
{
    // Инициализация карты персонажей
    characterImages = {
        {"fatima.png", ":/images/characters/fatima.png"},
        {"attila.png", ":/images/characters/attila.png"},
        {"alahverdi.png", ":/images/characters/alahverdi.png"},
        {"leyla.png", ":/images/characters/leyla.png"},
        {"rauf.png", ":/images/characters/rauf.png"}
    };

    // Инициализация статистики
    stats = {
        {"adrenaline", 0},
        {"hope", 0},
        {"anger", 0},
        {"pain", 0},
        {"cunning", 0},
        {"fear", 0},
        {"compassion", 0},
        {"guilt", 0},
        {"faith", 0},
        {"betrayal", 0}
    };

    setWindowTitle("Похищенная душа");
    resize(1024, 768);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupWelcomeScreen();
    setupGameScreen();
    setupEndingScreen();

    loadGameData();
}

void RomanceClub::setupWelcomeScreen()
{
    welcomeScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(welcomeScreen);

    QLabel *titleLabel = new QLabel("Добро пожаловать в");
    titleLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 28px;"
        "   font-weight: bold;"
        "   color: #e74c3c;"
        "   margin-bottom: 30px;"
        "}"
        );
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *welcomeImage = new QLabel();
    QPixmap welcomePix(":/images/backgrounds/welcome.png");
    if(welcomePix.isNull()) {
        qDebug() << "Не удалось загрузить welcome.png";
        welcomePix = QPixmap(800, 450);
        welcomePix.fill(Qt::darkGray);
    }
    welcomeImage->setPixmap(welcomePix.scaled(800, 450, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    welcomeImage->setAlignment(Qt::AlignCenter);

    QPushButton *startButton = new QPushButton("Начать игру");
    startButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 24px;"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   padding: 15px 30px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        );
    connect(startButton, &QPushButton::clicked, this, &RomanceClub::startGame);

    layout->addWidget(titleLabel);
    layout->addWidget(welcomeImage);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->setContentsMargins(50, 50, 50, 50);

    stack->addWidget(welcomeScreen);
}

void RomanceClub::setupGameScreen()
{
    gameScreen = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(gameScreen);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(20);

    backgroundLabel = new QLabel();
    backgroundLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(backgroundLabel);

    characterLabel = new QLabel();
    characterLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(characterLabel);

    textLabel = new QLabel();
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 18px;"
        "   color: white;"
        "   background-color: rgba(0,0,0,0.7);"
        "   padding: 20px;"
        "   border-radius: 10px;"
        "   margin: 0 50px;"
        "}"
        );
    contentLayout->addWidget(textLabel);

    statsLabel = new QLabel();
    statsLabel->setWordWrap(true);
    statsLabel->setAlignment(Qt::AlignLeft);
    statsLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 14px;"
        "   color: #f1c40f;"
        "   background-color: rgba(0,0,0,0.5);"
        "   padding: 10px;"
        "   border-radius: 5px;"
        "}"
        );
    contentLayout->addWidget(statsLabel);

    choicesLayout = new QVBoxLayout();
    choicesLayout->setAlignment(Qt::AlignCenter);
    choicesLayout->setSpacing(15);
    choicesLayout->setContentsMargins(100, 0, 100, 0);
    contentLayout->addLayout(choicesLayout);

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    stack->addWidget(gameScreen);
}

void RomanceClub::setupEndingScreen()
{
    endingScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(endingScreen);
    layout->setContentsMargins(0, 0, 0, 0);

    endingBackground = new QLabel();
    endingBackground->setAlignment(Qt::AlignCenter);
    layout->addWidget(endingBackground);

    endingTitle = new QLabel();
    endingTitle->setWordWrap(true);
    endingTitle->setAlignment(Qt::AlignCenter);
    endingTitle->setStyleSheet(
        "QLabel {"
        "   font-size: 32px;"
        "   font-weight: bold;"
        "   color: #f1c40f;"
        "   background-color: rgba(0,0,0,0.7);"
        "   padding: 20px;"
        "   border-radius: 10px;"
        "   margin: 20px 50px 0 50px;"
        "}"
        );

    endingText = new QLabel();
    endingText->setWordWrap(true);
    endingText->setAlignment(Qt::AlignCenter);
    endingText->setStyleSheet(
        "QLabel {"
        "   font-size: 24px;"
        "   color: white;"
        "   background-color: rgba(0,0,0,0.7);"
        "   padding: 30px;"
        "   border-radius: 15px;"
        "   margin: 0 50px 20px 50px;"
        "}"
        );

    restartButton = new QPushButton("Начать заново");
    restartButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 20px;"
        "   background-color: #3498db;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   margin-top: 20px;"
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        );
    connect(restartButton, &QPushButton::clicked, this, &RomanceClub::restartGame);

    layout->addWidget(endingTitle, 0, Qt::AlignCenter);
    layout->addWidget(endingText, 0, Qt::AlignCenter);
    layout->addWidget(restartButton, 0, Qt::AlignCenter);

    QPushButton* exitButton = new QPushButton("Выйти из игры");
    exitButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 20px;"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   margin-top: 10px;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        );
    connect(exitButton, &QPushButton::clicked, this, []() {
        QApplication::quit();
    });

    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    stack->addWidget(endingScreen);
}

void RomanceClub::loadGameData()
{
    QFile file(":/story.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка: Не удалось открыть story.json";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        qDebug() << "Ошибка: Неверный формат JSON";
        return;
    }

    QJsonObject root = doc.object();
    storyScenes = root["scenes"].toArray();
    endings = root["endings"].toArray();

    qDebug() << "Загружено сцен:" << storyScenes.size();
    qDebug() << "Загружено концовок:" << endings.size();
}

QJsonObject RomanceClub::findSceneById(const QString& sceneId)
{
    for (const QJsonValue& sceneValue : storyScenes) {
        QJsonObject scene = sceneValue.toObject();
        if (scene["id"].toString() == sceneId) {
            return scene;
        }
    }
    return QJsonObject();
}

void RomanceClub::updateStats(const QJsonObject& consequences)
{
    if (consequences.isEmpty()) return;

    QStringList statsText;
    for (auto it = consequences.begin(); it != consequences.end(); ++it) {
        QString statName = it.key();
        int value = it.value().toInt();

        if (stats.contains(statName)) {
            stats[statName] += value;
            QString arrow = value > 0 ? "↑" : "↓";
            statsText << QString("%1 %2%3").arg(statName).arg(arrow).arg(abs(value));
        }
    }

    statsLabel->setText("Изменения: " + statsText.join(", "));
}

void RomanceClub::startGame()
{
    for (auto& stat : stats) {
        stat = 0;
    }
    statsLabel->setText("");
    madeChoices.clear();
    currentSceneId = "awakening";
    showNextScene();
    stack->setCurrentWidget(gameScreen);
}

void RomanceClub::showNextScene()
{
    if (currentSceneId.isEmpty()) {
        qDebug() << "Ошибка: currentSceneId не установлен";
        return;
    }

    // Обработка финальной сцены
    if (currentSceneId.startsWith("ending_")) {
        QJsonObject ending;
        for (const QJsonValue& value : endings) {
            QJsonObject obj = value.toObject();
            if (obj["id"].toString() == currentSceneId) {
                ending = obj;
                break;
            }
        }
        if (!ending.isEmpty()) {
            showEnding(ending);
        } else {
            qDebug() << "Концовка не найдена:" << currentSceneId;
        }
        return;
    }

    QJsonObject scene = findSceneById(currentSceneId);
    if (scene.isEmpty()) {
        qDebug() << "Сцена не найдена:" << currentSceneId;
        return;
    }

    // Установка фона
    QString bgPath = ":/images/backgrounds/" + scene["background"].toString();
    QPixmap bgPix(bgPath);
    if (bgPix.isNull()) {
        qDebug() << "Не удалось загрузить фон:" << bgPath;
        bgPix = QPixmap(size());
        bgPix.fill(Qt::darkGray);
    }
    backgroundLabel->setPixmap(bgPix.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    // Установка персонажа
    if (scene.contains("character")) {
        QString charName = scene["character"].toString();
        if (characterImages.contains(charName)) {
            QPixmap charPix(characterImages[charName]);
            if (!charPix.isNull()) {
                characterLabel->setPixmap(charPix.scaledToHeight(400, Qt::SmoothTransformation));
                characterLabel->show();
            } else {
                qDebug() << "Не удалось загрузить персонажа:" << charName;
                characterLabel->hide();
            }
        }
    } else {
        characterLabel->hide();
    }

    // Установка текста
    textLabel->setText(scene["text"].toString());

    // Очистка предыдущих выборов
    QLayoutItem* item;
    while ((item = choicesLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    // Обработка вариантов выбора
    if (scene.contains("choices")) {
        QJsonArray choices = scene["choices"].toArray();
        qDebug() << "Создание кнопок:" << choices.size();

        for (const QJsonValue& choiceValue : choices) {
            QJsonObject choice = choiceValue.toObject();
            QPushButton* btn = new QPushButton(choice["text"].toString());

            QString buttonStyle = "QPushButton {"
                                  "   font-size: 16px;"
                                  "   color: white;"
                                  "   padding: 12px 24px;"
                                  "   border-radius: 6px;"
                                  "   margin: 10px;"
                                  "   min-width: 250px;"
                                  "}";

            if (currentSceneId == "wedding_escape") {
                buttonStyle += "QPushButton { background-color: #e74c3c; }"
                               "QPushButton:hover { background-color: #c0392b; }";
            } else {
                buttonStyle += "QPushButton { background-color: #3498db; }"
                               "QPushButton:hover { background-color: #2980b9; }";
            }

            btn->setStyleSheet(buttonStyle);

            if (choice.contains("thoughts")) {
                btn->setToolTip(choice["thoughts"].toString());
                btn->setToolTipDuration(3000);
            }

            connect(btn, &QPushButton::clicked, this, [this, choice]() {
                if (choice.contains("consequences")) {
                    updateStats(choice["consequences"].toObject());
                }

                if (choice.contains("id")) {
                    madeChoices.insert(choice["id"].toString());
                }

                currentSceneId = choice["nextScene"].toString();

                // Проверка условий для секретной концовки
                if (currentSceneId == "tragic_end") {
                    QStringList requiredChoices = {"attack", "reject", "fight"};
                    bool allMatched = std::all_of(requiredChoices.begin(), requiredChoices.end(),
                                                  [this](const QString& id) { return madeChoices.contains(id); });

                    if (allMatched && stats["faith"] >= 5) {
                        currentSceneId = "ending_secret";
                    }
                }

                showNextScene();
            });

            choicesLayout->addWidget(btn);
        }

    } else if (scene.contains("nextScene")) {
        // Одинарная сцена без выбора
        QPushButton* continueBtn = new QPushButton("Продолжить...");
        continueBtn->setStyleSheet(
            "QPushButton {"
            "   font-size: 16px;"
            "   background-color: #2ecc71;"
            "   color: white;"
            "   padding: 12px 24px;"
            "   border-radius: 6px;"
            "   margin: 10px;"
            "   min-width: 250px;"
            "}"
            "QPushButton:hover { background-color: #27ae60; }"
            );

        connect(continueBtn, &QPushButton::clicked, this, [this, scene]() {
            currentSceneId = scene["nextScene"].toString();

            // Проверка условий для секретной концовки
            if (currentSceneId == "tragic_end") {
                QStringList requiredChoices = {"attack", "reject", "fight"};
                bool allMatched = std::all_of(requiredChoices.begin(), requiredChoices.end(),
                                              [this](const QString& id) { return madeChoices.contains(id); });

                if (allMatched && stats["faith"] >= 5) {
                    currentSceneId = "ending_secret";
                }
            }

            showNextScene();
        });

        choicesLayout->addWidget(continueBtn);
    }
}


void RomanceClub::showEnding(const QJsonObject& ending)
{
    QString bgPath = ":/images/backgrounds/" + ending["background"].toString();
    QPixmap endPix(bgPath);
    if (endPix.isNull()) {
        qDebug() << "Не удалось загрузить фон концовки:" << bgPath;
        endPix = QPixmap(size());
        endPix.fill(Qt::black);
    }
    endingBackground->setPixmap(endPix.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    endingTitle->setText(ending["title"].toString());

    // Основное сообщение концовки
    QString endingMessage = ending["text"].toString();

    // Формирование текста о выборе игрока
    QString playerChoiceMessage = "Ваш выбор:\n";
    if (madeChoices.contains("attack")) {
        playerChoiceMessage += "Путь нападения.\n";
    }
    if (madeChoices.contains("reject")) {
        playerChoiceMessage += "Вы отвергли предложение.\n";
    }
    if (madeChoices.contains("fight")) {
        playerChoiceMessage += "Вы приняли бой.\n";
    }

    // Добавляем этот текст в основное сообщение концовки
    endingMessage += "\n" + playerChoiceMessage;

    // Добавляем финальное сообщение, в зависимости от концовки
    if (ending["id"].toString() == "ending_good") {
        endingMessage += "\nПоздравляем, вы обрели счастье! ❤️";
    }
    else if (ending["id"].toString() == "ending_bad") {
        endingMessage += "\nПовезёт в следующей жизни... 💔";
    }
    else if (ending["id"].toString() == "ending_secret") {
        endingMessage += "\nВы выбрали путь мести. ⚔️";
    }

    // Устанавливаем текст на экран
    endingText->setText(endingMessage);

    // Кнопки для перезапуска и выхода
    QPushButton* restartButton = new QPushButton("Начать заново");
    restartButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 20px;"
        "   background-color: #3498db;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   margin-top: 20px;"
        "} "
        "QPushButton:hover { background-color: #2980b9; }"
        );
    connect(restartButton, &QPushButton::clicked, this, &RomanceClub::restartGame);

    QPushButton* exitButton = new QPushButton("Выйти из игры");
    exitButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 20px;"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   padding: 10px 20px;"
        "   border-radius: 5px;"
        "   margin-top: 10px;"
        "} "
        "QPushButton:hover { background-color: #c0392b; }"
        );
    connect(exitButton, &QPushButton::clicked, this, []() {
        QApplication::quit();
    });

    // Создаём лэйаут для отображения концовки
    QVBoxLayout *endingLayout = new QVBoxLayout();
    endingLayout->addWidget(endingTitle);
    endingLayout->addWidget(endingText);
    endingLayout->addWidget(restartButton, 0, Qt::AlignCenter);
    endingLayout->addWidget(exitButton, 0, Qt::AlignCenter);
    endingLayout->addStretch();
    endingScreen->setLayout(endingLayout);

    stack->setCurrentWidget(endingScreen);
}


void RomanceClub::restartGame()
{
    stack->setCurrentWidget(welcomeScreen);
}

RomanceClub::~RomanceClub()
{
}
