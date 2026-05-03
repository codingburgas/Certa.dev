#pragma once

#include <QMainWindow>
#include "favorites.h"
#include "home.h"
#include "movies.h"
#include "profile.h"
#include "recommendations.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Layout;
}

QT_END_NAMESPACE

class Layout : public QMainWindow {
    Q_OBJECT

public:
    explicit Layout(QWidget *parent = nullptr);

    ~Layout() override;

private slots:
    void on_topBarSearchLineEdit_textEdited(const QString &text);

    void on_sidebarHomeButton_clicked();

    void on_sidebarMoviesButton_clicked();

    void on_sidebarRecommendationsButton_clicked();

    void on_sidebarFavoritesButton_clicked();

    void on_sidebarAuthButton_clicked();

    void on_sidebarSettingsButton_clicked();

private:
    enum class NavCurrentButtonIndex {
        Home,
        Movies,
        Recommendations,
        Favorites,
        Profile,
        Settings
    };

    void setNavActive(NavCurrentButtonIndex index);

    void refreshUserBadge();

    void openMovieFromOtherPage(int movieId);

    Ui::Layout *ui;
    Home *homePage;
    Movies *moviesPage;
    Recommendations *recommendationsPage;
    Favorites *favoritesPage;
    Profile *profilePage;
    Settings *settingsPage;
};
