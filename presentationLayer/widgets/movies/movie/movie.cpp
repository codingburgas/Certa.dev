#include "movie.h"
#include <QIcon>
#include <QPixmap>
#include <QScrollBar>
#include <QStyle>
#include "movieService.h"
#include "ui_movie.h"
#include "userSession.h"

Movie::Movie(QWidget *parent) : QWidget(parent), ui(new Ui::Movie) {
    ui->setupUi(this);

    QPixmap starPixmap(":/icons/star.svg");
    if (!starPixmap.isNull()) {
        ui->movieStarIcon->setPixmap(starPixmap.scaled(ICON_SIZE, ICON_SIZE, Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
    }
}

Movie::~Movie() {
    delete ui;
}

void Movie::loadMovie(int movieId) {
    currentMovieId = movieId;

    GetMovieResponse movieResponse = MovieService::getMovieById(movieId);
    if (!movieResponse.success) {
        ui->movieTitleLabel->setText(movieResponse.errorMessage);
        return;
    }

    renderMovie(movieResponse.movie);
    renderReviews();
    refreshAuthState();
    refreshFavoriteState();

    ui->reviewCommentEdit->clear();
    ui->reviewRatingSpinBox->setValue(8.0);
    setReviewStatus("", "");

    ui->movieScrollArea->verticalScrollBar()->setValue(0);
}

void Movie::setReviewStatus(const QString &text, const QString &state) {
    ui->reviewStatusLabel->setText(text);
    ui->reviewStatusLabel->setProperty("state", state);
    ui->reviewStatusLabel->style()->unpolish(ui->reviewStatusLabel);
    ui->reviewStatusLabel->style()->polish(ui->reviewStatusLabel);
}

void Movie::renderMovie(const MovieDto &movie) {
    ui->movieTitleLabel->setText(movie.title);
    ui->movieMetaLabel->setText(QString("%1  •  Directed by %2").arg(movie.year).arg(movie.director));
    ui->movieGenresLabel->setText(movie.genres);
    ui->movieRatingLabel->setText(QString::number(movie.rating, 'f', 1));
    ui->movieReviewCountLabel->setText(QString("(%1 reviews)").arg(movie.reviewCount));
    ui->movieDescriptionLabel->setText(movie.description);

    QPixmap poster(movie.posterPath);
    if (!poster.isNull()) {
        ui->moviePosterLabel->setPixmap(poster.scaled(POSTER_WIDTH, POSTER_HEIGHT, Qt::KeepAspectRatio,
                                                      Qt::SmoothTransformation));
    } else {
        ui->moviePosterLabel->setPixmap(QPixmap());
    }
}

void Movie::renderReviews() {
    clearReviewsLayout();

    GetReviewsResponse response = MovieService::getReviewsForMovie(currentMovieId);
    if (!response.success) {
        ui->reviewsEmptyLabel->setText(response.errorMessage);
        ui->reviewsEmptyLabel->setVisible(true);
        return;
    }

    if (response.reviews.isEmpty()) {
        ui->reviewsEmptyLabel->setText("No reviews yet. Be the first to leave one.");
        ui->reviewsEmptyLabel->setVisible(true);
        return;
    }

    ui->reviewsEmptyLabel->setVisible(false);

    for (int i = 0; i < response.reviews.size(); i++) {
        const Review &review = response.reviews[i];

        QFrame *item = new QFrame(this);
        item->setObjectName("reviewItem");

        QVBoxLayout *itemLayout = new QVBoxLayout(item);
        itemLayout->setContentsMargins(14, 12, 14, 12);
        itemLayout->setSpacing(6);

        QWidget *header = new QWidget(item);
        header->setStyleSheet("background-color: transparent;");
        QHBoxLayout *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(0, 0, 0, 0);
        headerLayout->setSpacing(8);

        QLabel *userLabel = new QLabel(review.username, header);
        userLabel->setObjectName("reviewItemUser");

        QLabel *ratingLabel = new QLabel(QString("★ %1").arg(QString::number(review.rating, 'f', 1)), header);
        ratingLabel->setObjectName("reviewItemRating");

        headerLayout->addWidget(userLabel);
        headerLayout->addStretch();
        headerLayout->addWidget(ratingLabel);

        itemLayout->addWidget(header);

        if (!review.comment.isEmpty()) {
            QLabel *commentLabel = new QLabel(review.comment, item);
            commentLabel->setObjectName("reviewItemComment");
            commentLabel->setWordWrap(true);
            itemLayout->addWidget(commentLabel);
        }

        ui->reviewsListLayout->addWidget(item);
    }
}

void Movie::clearReviewsLayout() {
    for (int i = ui->reviewsListLayout->count() - 1; i >= 0; i--) {
        QLayoutItem *item = ui->reviewsListLayout->takeAt(i);
        if (!item) continue;

        QWidget *widget = item->widget();
        if (widget) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Movie::refreshAuthState() {
    const bool isLoggedIn = UserSession::instance().isLoggedIn();
    ui->signInNoticeLabel->setVisible(!isLoggedIn);
}

void Movie::refreshFavoriteState() {
    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value() || currentMovieId < 0) {
        ui->movieFavoriteButton->setVisible(false);
        return;
    }

    ui->movieFavoriteButton->setVisible(true);

    FavoriteStatusResponse status = MovieService::isFavorite(currentUser->username, currentMovieId);
    updateFavoriteButton(status.success && status.isFavorite);
}

void Movie::updateFavoriteButton(bool isFavorite) {
    ui->movieFavoriteButton->setChecked(isFavorite);
    ui->movieFavoriteButton->setIcon(QIcon(isFavorite ? ":/icons/star-full.svg" : ":/icons/star.svg"));
    ui->movieFavoriteButton->setText(isFavorite ? "  Favorited" : "  Favorite");
}

void Movie::on_movieBackButton_clicked() {
    emit backRequested();
}

void Movie::on_movieFavoriteButton_clicked() {
    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value() || currentMovieId < 0) {
        return;
    }

    FavoriteStatusResponse status = MovieService::isFavorite(currentUser->username, currentMovieId);
    if (!status.success) {
        updateFavoriteButton(false);
        return;
    }

    if (status.isFavorite) {
        FavoriteResponse response = MovieService::removeFavorite(currentUser->username, currentMovieId);
        updateFavoriteButton(!response.success);
    } else {
        FavoriteResponse response = MovieService::addFavorite(currentUser->username, currentMovieId);
        updateFavoriteButton(response.success);
    }

    emit favoriteToggled();
}

void Movie::on_submitReviewButton_clicked() {
    setReviewStatus("", "");

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value()) {
        setReviewStatus("Please sign in to leave a review.", "error");
        return;
    }

    if (currentMovieId < 0) {
        setReviewStatus("No movie selected.", "error");
        return;
    }

    const double rating = ui->reviewRatingSpinBox->value();
    const QString comment = ui->reviewCommentEdit->toPlainText().trimmed();

    ReviewResponse response = MovieService::createReview(currentUser->username, currentMovieId, rating, comment);
    if (!response.success) {
        setReviewStatus(response.errorMessage, "error");
        return;
    }

    ui->reviewCommentEdit->clear();
    ui->reviewRatingSpinBox->setValue(8.0);
    setReviewStatus("Review submitted.", "success");

    GetMovieResponse movieResponse = MovieService::getMovieById(currentMovieId);
    if (movieResponse.success) {
        renderMovie(movieResponse.movie);
    }
    renderReviews();

    emit reviewSubmitted();
}
