#include "movies.h"
#include <QPixmap>
#include "movieService.h"
#include "ui_movies.h"

Movies::Movies(QWidget *parent) : QWidget(parent), ui(new Ui::Movies) {
    ui->setupUi(this);

    for (int col = 0; col < COLUMNS; col++) {
        ui->moviesGrid->setColumnStretch(col, 1);
    }

    loadMovies();
}

Movies::~Movies() {
    delete ui;
}

void Movies::loadMovies() {
    GetMoviesResponse response = MovieService::getAllMovies();
    if (!response.success) {
        qDebug() << "Failed to load movies:" << response.errorMessage;
        return;
    }

    const QVector<Movie> movies = response.movies;
    for (int i = 0; i < movies.size(); i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        ui->moviesGrid->addWidget(createMovieCard(movies[i]), row, col);
    }

    int lastRow = (movies.size() - 1) / COLUMNS + 1;
    ui->moviesGrid->setRowStretch(lastRow, 1);
}

QWidget *Movies::createMovieCard(const Movie &movie) {
    QFrame *card = new QFrame(this);
    card->setObjectName("movieCard");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(8, 8, 8, 0);
    cardLayout->setSpacing(0);

    QLabel *posterLabel = new QLabel(card);
    posterLabel->setFixedHeight(POSTER_HEIGHT);
    posterLabel->setAlignment(Qt::AlignCenter);
    posterLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    QPixmap pixmap(movie.posterPath);
    if (!pixmap.isNull()) {
        posterLabel->setPixmap(pixmap.scaledToHeight(POSTER_HEIGHT, Qt::SmoothTransformation));
    }

    QWidget *infoWidget = new QWidget(card);
    infoWidget->setStyleSheet("background-color: transparent;");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(10, 8, 10, 10);
    infoLayout->setSpacing(6);

    QLabel *titleLabel = new QLabel(movie.title, infoWidget);
    titleLabel->setObjectName("movieCardTitle");
    titleLabel->setWordWrap(true);

    QLabel *genresLabel = new QLabel(movie.genres, infoWidget);
    genresLabel->setObjectName("movieCardGenres");
    genresLabel->setWordWrap(true);

    QWidget *statsRow = new QWidget(infoWidget);
    statsRow->setStyleSheet("background-color: transparent;");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsRow);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(4);

    QLabel *starIcon = new QLabel(statsRow);
    QPixmap starPixmap(":/icons/star.svg");
    if (!starPixmap.isNull()) {
        starIcon->setPixmap(starPixmap.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    starIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *ratingLabel = new QLabel(QString::number(movie.rating, 'f', 1), statsRow);
    ratingLabel->setObjectName("movieCardRating");

    QLabel *commentsIcon = new QLabel(statsRow);
    QPixmap commentsPixmap(":/icons/comments.svg");
    if (!commentsPixmap.isNull()) {
        commentsIcon->setPixmap(commentsPixmap.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    commentsIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *reviewCountLabel = new QLabel(QString::number(movie.reviewCount), statsRow);
    reviewCountLabel->setObjectName("movieCardReviews");

    statsLayout->addWidget(starIcon);
    statsLayout->addWidget(ratingLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(commentsIcon);
    statsLayout->addWidget(reviewCountLabel);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(genresLabel);
    infoLayout->addWidget(statsRow);

    cardLayout->addWidget(posterLabel);
    cardLayout->addWidget(infoWidget);

    return card;
}
