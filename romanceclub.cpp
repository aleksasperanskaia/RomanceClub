#include "romanceclub.h"
#include <QDebug>
#include <QPushButton>
#include <QFile>
#include <QDir>

RomanceClub::RomanceClub(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
}

void RomanceClub::setupUI()
{
    // Set window properties
    setWindowTitle("Romance Club");
    resize(800, 600);

    // Create central widget and layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);

    // Create background view
    backgroundScene = new QGraphicsScene(this);
    backgroundView = new QGraphicsView(backgroundScene);
    backgroundView->setFixedHeight(300);
    backgroundView->setStyleSheet("border: 1px solid gray;");

    // Create character view
    characterScene = new QGraphicsScene(this);
    characterView = new QGraphicsView(characterScene);
    characterView->setFixedHeight(200);
    characterView->setStyleSheet("border: 1px solid gray;");

    // Create text label
    textLabel = new QLabel();
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("font-size: 14px; padding: 10px;");
    textLabel->setAlignment(Qt::AlignCenter);

    // Create choices layout
    choicesLayout = new QVBoxLayout();

    // Add widgets to main layout
    mainLayout->addWidget(backgroundView);
    mainLayout->addWidget(characterView);
    mainLayout->addWidget(textLabel);
    mainLayout->addLayout(choicesLayout);
    mainLayout->addStretch();

    // Set central widget
    setCentralWidget(centralWidget);
}

void RomanceClub::loadStory()
{
    QFile file(":/story.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open story file:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    if (doc.isNull()) {
        qWarning() << "Invalid JSON file";
        return;
    }
    scenes = doc.object()["scenes"].toArray();
}

void RomanceClub::start()
{
    showScene("start");
}

void RomanceClub::showScene(const QString &sceneId)
{
    currentScene = findSceneById(sceneId);
    if (currentScene.isEmpty()) {
        qWarning() << "Scene not found:" << sceneId;
        return;
    }

    // Display scene text
    textLabel->setText(currentScene["text"].toString());

    // Clear previous choices
    clearChoices();

    // Add new choices
    QJsonArray choices = currentScene["choices"].toArray();
    for (int i = 0; i < choices.size(); ++i) {
        QJsonObject choice = choices[i].toObject();
        QPushButton *button = new QPushButton(choice["text"].toString());
        connect(button, &QPushButton::clicked, this, [this, i]() { handleChoice(i); });
        choicesLayout->addWidget(button);
    }

    // Set background image based on scene
    QString bgImage;
    if (sceneId == "start" || sceneId == "scream" || sceneId == "fight_back") {
        bgImage = ":/images/backgrounds/street.jpg";
    } else if (sceneId == "surrender" || sceneId == "struggle_again" ||
               sceneId == "allahverdi_help" || sceneId == "warn_allahverdi") {
        bgImage = ":/images/backgrounds/room.jpg";
    } else if (sceneId == "run_away" || sceneId == "give_up" ||
               sceneId == "escape_with_leyla" || sceneId == "fight_atilla") {
        bgImage = ":/images/backgrounds/car.jpg";
    } else if (sceneId == "grief_escape" || sceneId == "bad_end_drown") {
        bgImage = ":/images/backgrounds/sea.jpg";
    } else if (sceneId == "foreign_start" || sceneId == "trap_atilla" ||
               sceneId == "bad_end_escape" || sceneId == "good_ending") {
        bgImage = ":/images/backgrounds/wedding.jpg";
    }

    if (!bgImage.isEmpty()) {
        backgroundScene->clear();
        QPixmap bgPixmap(bgImage);
        if (!bgPixmap.isNull()) {
            backgroundScene->addPixmap(bgPixmap.scaled(backgroundView->width(), backgroundView->height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        } else {
            qWarning() << "Failed to load background image:" << bgImage;
        }
    }

    // Set character image based on scene
    QString charImage;
    if (sceneId == "start" || sceneId == "scream" || sceneId == "fight_back") {
        charImage = ":/images/characters/attila.png";
    } else if (sceneId.contains("allahverdi")) {
        charImage = ":/images/characters/allahverdi.png";
    } else if (sceneId.contains("leyla")) {
        charImage = ":/images/characters/leyla.png";
    } else if (sceneId.contains("rauf")) {
        charImage = ":/images/characters/rauf.png";
    } else {
        charImage = ":/images/characters/fatima.png";
    }

    characterScene->clear();
    QPixmap charPixmap(charImage);
    if (!charPixmap.isNull()) {
        characterScene->addPixmap(charPixmap.scaled(characterView->width(), characterView->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qWarning() << "Failed to load character image:" << charImage;
    }
}

void RomanceClub::handleChoice(int index)
{
    QJsonArray choices = currentScene["choices"].toArray();
    if (index >= 0 && index < choices.size()) {
        QJsonObject choice = choices[index].toObject();
        QString nextSceneId = choice["next_scene"].toString();
        showScene(nextSceneId);
    }
}

void RomanceClub::clearChoices()
{
    QLayoutItem *item;
    while ((item = choicesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

QJsonObject RomanceClub::findSceneById(const QString &id)
{
    for (const QJsonValue &sceneValue : scenes) {
        QJsonObject scene = sceneValue.toObject();
        if (scene["id"].toString() == id) {
            return scene;
        }
    }
    return QJsonObject();
}
