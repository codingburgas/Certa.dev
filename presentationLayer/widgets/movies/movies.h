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

    void showList();

    void openMoviePage(int movieId);

private:
    const int COLUMNS = 3;

    Ui::Movies *ui;
    Movie *moviePage;

    void loadMovies();

    void reloadMovies();
};
