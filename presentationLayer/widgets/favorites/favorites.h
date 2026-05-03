#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Favorites;
}

QT_END_NAMESPACE

class Favorites : public QWidget {
    Q_OBJECT

public:
    explicit Favorites(QWidget *parent = nullptr);

    ~Favorites() override;

    void refresh();

signals:
    void movieClicked(int movieId);

private:
    const int COLUMNS = 3;

    Ui::Favorites *ui;

    void clearGrid();
};
