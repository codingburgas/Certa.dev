#pragma once

#include <QFrame>
#include "movieDto.h"

class MovieCard : public QFrame {
    Q_OBJECT

public:
    explicit MovieCard(const MovieDto &movie, QWidget *parent = nullptr);

signals:
    void clicked(int movieId);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int POSTER_HEIGHT = 260;
    const int ICON_SIZE = 14;

    int _movieId;
};
