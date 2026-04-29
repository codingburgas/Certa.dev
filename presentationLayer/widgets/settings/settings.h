#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Settings;
}
QT_END_NAMESPACE

class Settings : public QWidget {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_saveUsernameButton_clicked();
    void on_savePasswordButton_clicked();
    void on_saveInterestsButton_clicked();
    void on_termsViewButton_clicked();
    void on_privacyViewButton_clicked();
    void on_termsBackButton_clicked();
    void on_privacyBackButton_clicked();
    void on_deleteAccountButton_clicked();

private:
    void refreshUserState();
    void loadGenres();
    void applyInterestsSelection();
    void clearGenresGrid();

    Ui::Settings *ui;
};

