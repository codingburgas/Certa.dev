#include "movies.h"
#include "movie.h"
#include "movieCard.h"
#include "movieService.h"
#include "ui_movies.h"

Movies::Movies(QWidget *parent) : QWidget(parent), ui(new Ui::Movies) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->moviesGrid->setColumnStretch(col, 1);
    }

    moviePage = new Movie(this);
    ui->moviePageHostLayout->addWidget(moviePage);
    connect(moviePage, &Movie::backRequested, this, &Movies::showList);
    connect(moviePage, &Movie::reviewSubmitted, this, &Movies::reloadMovies);

    showList();

    loadMovies();
}

Movies::~Movies() {
    delete ui;
}

void Movies::showList() {
    ui->moviesStackedWidget->setCurrentWidget(ui->moviesListPage);
}

void Movies::openMoviePage(int movieId) {
    moviePage->loadMovie(movieId);
    ui->moviesStackedWidget->setCurrentWidget(ui->moviePageHost);
}

void Movies::loadMovies() {
    GetMoviesResponse response = MovieService::getAllMovies();
    if (!response.success) {
        qDebug() << "Failed to load movies:" << response.errorMessage;
        return;
    }

    const QVector<MovieDto> movies = response.movies;
    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        MovieCard *card = new MovieCard(movies[i], this);
        connect(card, &MovieCard::clicked, this, &Movies::openMoviePage);
        ui->moviesGrid->addWidget(card, row, col);
    }

    int lastRow = (movies.size() - 1) / COLUMNS + 1;
    ui->moviesGrid->setRowStretch(lastRow, 1);
}

void Movies::reloadMovies() {
    QLayoutItem *item;
    while ((item = ui->moviesGrid->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    loadMovies();
}
