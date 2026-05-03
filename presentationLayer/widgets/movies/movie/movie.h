#pragma once

#include <QWidget>
#include "movieDto.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Movie;
}

QT_END_NAMESPACE

class Movie : public QWidget {
    Q_OBJECT

public:
    explicit Movie(QWidget *parent = nullptr);

    ~Movie() override;

    void loadMovie(int movieId);

signals:
    void backRequested();

    void reviewSubmitted();

    void favoriteToggled();

private slots:
    void on_movieBackButton_clicked();

    void on_submitReviewButton_clicked();

    void on_movieFavoriteButton_clicked();

private:
    const int POSTER_WIDTH = 220;
    const int POSTER_HEIGHT = 320;
    const int ICON_SIZE = 16;

    Ui::Movie *ui;
    int currentMovieId = -1;

    void renderMovie(const MovieDto &movie);

    void renderReviews();

    void clearReviewsLayout();

    void refreshAuthState();

    void setReviewStatus(const QString &text, const QString &state);

    void refreshFavoriteState();

    void updateFavoriteButton(bool isFavorite);
};
