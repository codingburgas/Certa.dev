#include "layout.h"
#include <QFontMetrics>
#include "auth.h"
#include "ui_layout.h"
#include "userSession.h"

Layout::Layout(QWidget *parent) : QMainWindow(parent), ui(new Ui::Layout) {
    ui->setupUi(this);

    homePage = new Home(this);
    ui->homePageLayout->addWidget(homePage);
    connect(homePage, &Home::signInRequested, this, &Layout::on_sidebarAuthButton_clicked);
    connect(homePage, &Home::exploreRecommendationsRequested, this, &Layout::on_sidebarRecommendationsButton_clicked);
    connect(homePage, &Home::movieClicked, this, &Layout::openMovieFromOtherPage);

    moviesPage = new Movies(this);
    ui->moviesPageLayout->addWidget(moviesPage);

    recommendationsPage = new Recommendations(this);
    ui->recommendationsPageLayout->addWidget(recommendationsPage);
    connect(recommendationsPage, &Recommendations::movieClicked, this, &Layout::openMovieFromOtherPage);

    profilePage = new Profile(this);
    ui->profilePageLayout->addWidget(profilePage);
    connect(profilePage, &Profile::logoutRequested, this, [this]() {
        UserSession::instance().clearUser();
        ui->sidebarAuthButton->setText("  Sign In / Sign Up");
        refreshUserBadge();
        homePage->refresh();
        on_sidebarHomeButton_clicked();
    });

    settingsPage = new Settings(this);
    ui->settingsPageLayout->addWidget(settingsPage);

    if (UserSession::instance().isLoggedIn()) {
        ui->sidebarAuthButton->setText("  Profile");
    }

    refreshUserBadge();

    on_sidebarHomeButton_clicked();
}

void Layout::refreshUserBadge() {
    const auto currentUser = UserSession::instance().getCurrentUser();
    const QString name = currentUser ? currentUser->username : QStringLiteral("Guest");
    const QFontMetrics metrics(ui->topBarUsernameLabel->font());
    ui->topBarUsernameLabel->setText(metrics.elidedText(name, Qt::ElideRight, ui->topBarUsernameLabel->width()));

    const QString initial = name.isEmpty() ? "?" : name.left(1).toUpper();
    ui->topBarAvatarLabel->setText(initial);
}

void Layout::on_topBarSearchLineEdit_textEdited(const QString &text) {
    Q_UNUSED(text); // will be deleted later when search is made
    on_sidebarMoviesButton_clicked();
}

Layout::~Layout() {
    delete ui;
}

void Layout::setNavActive(NavCurrentButtonIndex index) {
    ui->sidebarHomeButton->setChecked(index == NavCurrentButtonIndex::Home);
    ui->sidebarMoviesButton->setChecked(index == NavCurrentButtonIndex::Movies);
    ui->sidebarRecommendationsButton->setChecked(index == NavCurrentButtonIndex::Recommendations);
    ui->sidebarAuthButton->setChecked(index == NavCurrentButtonIndex::Profile);
    ui->sidebarSettingsButton->setChecked(index == NavCurrentButtonIndex::Settings);
}

void Layout::openMovieFromOtherPage(int movieId) {
    setNavActive(NavCurrentButtonIndex::Movies);
    moviesPage->openMoviePage(movieId);
    ui->stackedWidget->setCurrentWidget(ui->moviesStackPage);
}

void Layout::on_sidebarHomeButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Home);
    homePage->refresh();
    ui->stackedWidget->setCurrentWidget(ui->homeStackPage);
}

void Layout::on_sidebarMoviesButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Movies);
    moviesPage->showList();
    ui->stackedWidget->setCurrentWidget(ui->moviesStackPage);
}

void Layout::on_sidebarRecommendationsButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Recommendations);
    recommendationsPage->refresh();
    ui->stackedWidget->setCurrentWidget(ui->recommendationsStackPage);
}

void Layout::on_sidebarAuthButton_clicked() {
    if (UserSession::instance().isLoggedIn()) {
        setNavActive(NavCurrentButtonIndex::Profile);
        ui->stackedWidget->setCurrentWidget(ui->profileStackPage);
    } else {
        this->hide();
        auto authPage = new Auth();
        authPage->show();
    }
}

void Layout::on_sidebarSettingsButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Settings);
    ui->stackedWidget->setCurrentWidget(ui->settingsStackPage);
}
