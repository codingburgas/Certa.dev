#include "interestsDialog.h"
#include <QCheckBox>
#include "layout.h"
#include "ui_interestsDialog.h"

InterestsDialog::InterestsDialog(QWidget *parent) : QWidget(parent), ui(new Ui::InterestsDialog) {
    ui->setupUi(this);

    // TODO fetch genres from db later
    const QVector<QString> genres = {
        "Action", "Adventure", "Animation", "Biography", "Comedy", "Crime",
        "Documentary", "Drama", "Family", "Fantasy", "History", "Horror",
        "Music", "Mystery", "Romance", "Science Fiction", "Sport", "Thriller"
    };

    for (int i = ui->genresGridLayout->count() - 1; i >= 0; i--) {
        QLayoutItem *item = ui->genresGridLayout->takeAt(i);

        if (item == nullptr) {
            continue;
        }

        QWidget *widget = item->widget();
        if (widget != nullptr) {
            widget->deleteLater();
        }

        delete item;
    }

    for (int i = 0; i < genres.size(); i++) {
        auto *genreCheckBox = new QCheckBox(genres[i], this);
        genreCheckBox->setMinimumHeight(36);
        ui->genresGridLayout->addWidget(genreCheckBox, i / 4, i % 4);
    }
}

InterestsDialog::~InterestsDialog() {
    delete ui;
}

void InterestsDialog::on_saveButton_clicked() {
    QVector<QString> selectedGenres;

    for (int i = 0; i < ui->genresGridLayout->count(); i++) {
        QLayoutItem *item = ui->genresGridLayout->itemAt(i);

        auto *checkBox = qobject_cast<QCheckBox *>(item ? item->widget() : nullptr);

        if (checkBox && checkBox->isChecked()) {
            selectedGenres.append(checkBox->text());
        }
    }

    // TODO save selectedGenres to db later

    this->hide();
    auto layout = new Layout();
    layout->show();
}

void InterestsDialog::on_skipButton_clicked() {
    // TODO use empty list to save in db later

    this->hide();
    auto layout = new Layout();
    layout->show();
}
