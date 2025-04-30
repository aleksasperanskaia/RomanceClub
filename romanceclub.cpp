#include "romanceclub.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QEasingCurve>
#include <QPropertyAnimation>
#include <QDebug>
#include <QPixmap>
#include <QStackedWidget>


RomanceClub::RomanceClub(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Добро пожаловать в Азербайджанский Romance Club");
    resize(1024, 768);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupWelcomeScreen();
    setupGameScreen();
    setupEndingScreen();

    loadGameData();
}

RomanceClub::~RomanceClub()
{
}

void RomanceClub::setupWelcomeScreen()
{
    welcomeScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(welcomeScreen);

    QLabel *welcomeImage = new QLabel();
    welcomeImage->setPixmap(QPixmap(":/images/backgrounds/street.png").scaled(800, 450, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    welcomeImage->setAlignment(Qt::AlignCenter);

    QPushButton *startButton = new QPushButton("Начать");
    startButton->setStyleSheet(
        "QPushButton {"
        " font-size: 24px;"
        " background-color: #e74c3c;"
        " color: white;"
        " padding: 15px 30px;"
        " border-radius: 10px;"
        "}"
        );
    connect(startButton, &QPushButton::clicked, this, &RomanceClub::startGame);

    layout->addWidget(welcomeImage);
    layout->addWidget(startButton, 0, Qt::AlignCenter);

    stack->addWidget(welcomeScreen);
}

void RomanceClub::setupGameScreen()
{
    gameScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(gameScreen);

    backgroundLabel = new QLabel();
    backgroundLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(backgroundLabel);

    characterLabel = new QLabel();
    characterLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(characterLabel);

    textLabel = new QLabel();
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("font-size: 18px; color: white; background-color: rgba(0,0,0,0.7); padding: 20px;");
    layout->addWidget(textLabel);

    choicesLayout = new QVBoxLayout();
    layout->addLayout(choicesLayout);

    stack->addWidget(gameScreen);
}

void RomanceClub::setupEndingScreen()
{
    endingScreen = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(endingScreen);

    endingBackground = new QLabel();
    endingBackground->setAlignment(Qt::AlignCenter);
    layout->addWidget(endingBackground);

    endingText = new QLabel();
    endingText->setWordWrap(true);
    endingText->setAlignment(Qt::AlignCenter);
    endingText->setStyleSheet("font-size: 24px; color: white; background-color: rgba(0,0,0,0.7); padding: 30px;");

    restartButton = new QPushButton("Начать сначала");
    restartButton->setStyleSheet(
        "QPushButton {"
        " font-size: 20px;"
        " background-color: #3498db;"
        " color: white;"
        " padding: 10px 20px;"
        " border-radius: 5px;"
        "}"
        );
    connect(restartButton, &QPushButton::clicked, this, &RomanceClub::restartGame);

    layout->addWidget(endingText);
    layout->addWidget(restartButton, 0, Qt::AlignCenter);

    stack->addWidget(endingScreen);
}

void RomanceClub::loadGameData()
{
    QFile file(":/story.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open story.json";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        qDebug() << "Invalid JSON format";
        return;
    }

    storyScenes = doc.array();
    qDebug() << "Loaded" << storyScenes.size() << "scenes";
}

void RomanceClub::startGame()
{
    currentSceneIndex = 0;
    stack->setCurrentWidget(gameScreen);
    showNextScene();
}

void RomanceClub::showNextScene()
{
    if (currentSceneIndex < storyScenes.size()) {
        QJsonObject scene = storyScenes[currentSceneIndex].toObject();

        // Set background
        QString bgImage = ":/images/backgrounds/" + scene["background"].toString() + ".png";
        backgroundLabel->setPixmap(QPixmap(bgImage).scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

        // Set character
        QString charImage = ":/images/characters/" + scene["character"].toString() + ".png";
        characterLabel->setPixmap(QPixmap(charImage).scaledToHeight(500, Qt::SmoothTransformation));

        // Set text
        textLabel->setText(scene["text"].toString());

        // Clear previous choices
        QLayoutItem* item;
        while ((item = choicesLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        }

        // Add new choices
        QJsonArray choices = scene["choices"].toArray();
        for (int i = 0; i < choices.size(); ++i) {
            QPushButton* btn = new QPushButton(choices[i].toString());
            btn->setProperty("choiceIndex", i);
            connect(btn, &QPushButton::clicked, this, [this, i]() { makeChoice(i); });
            choicesLayout->addWidget(btn);
        }
    } else {
        showEnding(true); // Default to good ending if no choices made
    }
}

void RomanceClub::makeChoice(int choiceIndex)
{
    // For demo purposes, first choice leads to good ending
    if (currentSceneIndex == storyScenes.size() - 1) {
        showEnding(choiceIndex == 0);
    } else {
        currentSceneIndex++;
        showNextScene();
    }
}

void RomanceClub::showEnding(bool isGoodEnding)
{
    if (isGoodEnding) {
        endingBackground->setPixmap(QPixmap(":/images/backgrounds/wedding.jpg").scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        endingText->setText("Поздравляем! Вы дошли до хорошего конца!");
    } else {
        endingBackground->setPixmap(QPixmap(":/images/backgrounds/sea.jpg").scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        endingText->setText("Конец... Повезет в следующей жизни");
    }

    stack->setCurrentWidget(endingScreen);
}

void RomanceClub::restartGame()
{
    stack->setCurrentWidget(welcomeScreen);
}
