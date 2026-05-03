#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Recommendations;
}

QT_END_NAMESPACE

class Recommendations : public QWidget {
    Q_OBJECT

public:
    explicit Recommendations(QWidget *parent = nullptr);

    ~Recommendations() override;

    void refresh();

signals:
    void movieClicked(int movieId);

private:
    const int COLUMNS = 3;

    Ui::Recommendations *ui;

    void clearGrid();
};
