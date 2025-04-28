#ifndef ROMANCECLUB_H
#define ROMANCECLUB_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QJsonArray>

class QLabel;
class QPushButton;
class QVBoxLayout;

class RomanceClub : public QMainWindow
{
    Q_OBJECT

public:
    RomanceClub(QWidget *parent = nullptr);

private slots:
    void startGame();
    void showNextScene();
    void makeChoice(int choiceIndex);
    void showEnding(bool isGoodEnding);
    void restartGame();

private:
    void setupWelcomeScreen();
    void setupGameScreen();
    void setupEndingScreen();
    void loadGameData();

    QStackedWidget *stack;
    QWidget *welcomeScreen;
    QWidget *gameScreen;
    QWidget *endingScreen;

    // Game screen
    QLabel *backgroundLabel;
    QLabel *characterLabel;
    QLabel *textLabel;
    QVBoxLayout *choicesLayout;

    // Ending screen
    QLabel *endingBackground;
    QLabel *endingText;
    QPushButton *restartButton;

    QJsonArray storyScenes;
    int currentSceneIndex = 0;
};

#endif // ROMANCECLUB_H
