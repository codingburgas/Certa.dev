#include "favorites.h"
#include "movieCard.h"
#include "movieService.h"
#include "ui_favorites.h"
#include "userSession.h"

Favorites::Favorites(QWidget *parent) : QWidget(parent), ui(new Ui::Favorites) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->favoritesGrid->setColumnStretch(col, 1);
    }
}

Favorites::~Favorites() {
    delete ui;
}

void Favorites::clearGrid() {
    QLayoutItem *item;
    while ((item = ui->favoritesGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Favorites::refresh() {
    clearGrid();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser) {
        ui->favoritesEmptyLabel->setText("Sign in to save and view your favorite movies.");
        ui->favoritesStack->setCurrentWidget(ui->favoritesEmptyPage);
        return;
    }

    GetMoviesResponse response = MovieService::getFavoriteMovies(currentUser->username);
    if (!response.success) {
        qDebug() << "Failed to load favorites:" << response.errorMessage;
        ui->favoritesEmptyLabel->setText("Failed to load favorites.");
        ui->favoritesStack->setCurrentWidget(ui->favoritesEmptyPage);
        return;
    }

    if (response.movies.isEmpty()) {
        ui->favoritesEmptyLabel->setText("No favorites yet. Open a movie and tap the star to add it here.");
        ui->favoritesStack->setCurrentWidget(ui->favoritesEmptyPage);
        return;
    }

    ui->favoritesStack->setCurrentWidget(ui->favoritesGridPage);

    const QVector<MovieDto> movies = response.movies;
    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Favorites::movieClicked);
        ui->favoritesGrid->addWidget(card, row, col);
    }

    int lastRow = (movies.size() - 1) / COLUMNS + 1;
    ui->favoritesGrid->setRowStretch(lastRow, 1);
}
