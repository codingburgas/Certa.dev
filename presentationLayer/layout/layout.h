#pragma once

#include <QMainWindow>
#include "home.h"
#include "movies.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Layout;
}

QT_END_NAMESPACE

class Layout : public QMainWindow {
    Q_OBJECT

public:
    Layout(QWidget *parent = nullptr);

    ~Layout();

private slots:
    void on_sidebarHomeButton_clicked();

    void on_sidebarMoviesButton_clicked();

    void on_sidebarLoginButton_clicked();

    void on_sidebarSettingsButton_clicked();

private:
    enum class NavPage {
        Home,
        Movies,
        Login,
        Settings
    };

    void setNavActive(NavPage page);

    Ui::Layout *ui;
    Home *homePage;
    Movies *moviesPage;
};
