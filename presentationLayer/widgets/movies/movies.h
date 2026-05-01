#pragma once

#include <QWidget>
#include "movie.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Movies;
}
QT_END_NAMESPACE

class Movies : public QWidget {
    Q_OBJECT

public:
    explicit Movies(QWidget *parent = nullptr);
    ~Movies() override;

private:
    const int COLUMNS = 3;
    const int POSTER_HEIGHT = 260;
    const int ICON_SIZE = 14;

    Ui::Movies *ui;

    void loadMovies();
    QWidget *createMovieCard(const Movie &movie);
};
