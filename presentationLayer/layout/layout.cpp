#include "layout.h"
#include "auth.h"
#include "ui_layout.h"

Layout::Layout(QWidget *parent) : QMainWindow(parent), ui(new Ui::Layout) {
    ui->setupUi(this);

    homePage = new Home(this);
    ui->homePageLayout->addWidget(homePage);

    moviesPage = new Movies(this);
    ui->moviesPageLayout->addWidget(moviesPage);

    on_sidebarHomeButton_clicked();
}

Layout::~Layout() {
    delete ui;
}

void Layout::setNavActive(NavCurrentButtonIndex index) {
    ui->sidebarHomeButton->setChecked(index == NavCurrentButtonIndex::Home);
    ui->sidebarMoviesButton->setChecked(index == NavCurrentButtonIndex::Movies);
    ui->sidebarSettingsButton->setChecked(index == NavCurrentButtonIndex::Settings);
}

void Layout::on_sidebarHomeButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Home);
    ui->stackedWidget->setCurrentWidget(ui->homeStackPage);
}

void Layout::on_sidebarMoviesButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Movies);
    ui->stackedWidget->setCurrentWidget(ui->moviesStackPage);
}

void Layout::on_sidebarAuthButton_clicked() {
    this->hide();
    auto authPage = new Auth();
    authPage->show();
}

void Layout::on_sidebarSettingsButton_clicked() {
    setNavActive(NavCurrentButtonIndex::Settings);
}
