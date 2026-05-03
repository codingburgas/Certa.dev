#include "recommendations.h"
#include "movieCard.h"
#include "movieService.h"
#include "ui_recommendations.h"
#include "userSession.h"

Recommendations::Recommendations(QWidget *parent) : QWidget(parent), ui(new Ui::Recommendations) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->recommendationsGrid->setColumnStretch(col, 1);
    }
}

Recommendations::~Recommendations() {
    delete ui;
}

void Recommendations::clearGrid() {
    QLayoutItem *item;
    while ((item = ui->recommendationsGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Recommendations::refresh() {
    clearGrid();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser) {
        ui->recommendationsStack->setCurrentWidget(ui->recommendationsEmptyPage);
        return;
    }

    GetMoviesResponse response = MovieService::getRecommendedMovies(currentUser->username);
    if (!response.success) {
        qDebug() << "Failed to load recommendations:" << response.errorMessage;
        ui->recommendationsStack->setCurrentWidget(ui->recommendationsEmptyPage);
        return;
    }

    if (response.movies.isEmpty()) {
        ui->recommendationsStack->setCurrentWidget(ui->recommendationsEmptyPage);
        return;
    }

    ui->recommendationsStack->setCurrentWidget(ui->recommendationsGridPage);

    const QVector<MovieDto> movies = response.movies;
    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Recommendations::movieClicked);
        ui->recommendationsGrid->addWidget(card, row, col);
    }

    int lastRow = (movies.size() - 1) / COLUMNS + 1;
    ui->recommendationsGrid->setRowStretch(lastRow, 1);
}
