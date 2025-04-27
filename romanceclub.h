#ifndef ROMANCECLUB_H
#define ROMANCECLUB_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class RomanceClub : public QMainWindow
{
    Q_OBJECT

public:
    RomanceClub(QWidget *parent = nullptr);
    void loadStory();
    void start();
    void showScene(const QString &sceneId);

private slots:
    void handleChoice(int index);

private:
    QGraphicsScene *backgroundScene;
    QGraphicsView *backgroundView;
    QGraphicsScene *characterScene;
    QGraphicsView *characterView;
    QLabel *textLabel;
    QVBoxLayout *mainLayout;
    QWidget *centralWidget;
    QVBoxLayout *choicesLayout;

    QJsonArray scenes;
    QJsonObject currentScene;

    void clearChoices();
    void setupUI();
    QJsonObject findSceneById(const QString &id);
};

#endif // ROMANCECLUB_H
