#include "interestsDialog.h"
#include <QCheckBox>
#include "layout.h"
#include "movieService.h"
#include "ui_interestsDialog.h"
#include "userService.h"
#include "userSession.h"

InterestsDialog::InterestsDialog(QWidget *parent) : QWidget(parent), ui(new Ui::InterestsDialog) {
    ui->setupUi(this);

    MovieResponse genresResponse = MovieService::getAllGenres();

    if (!genresResponse.success) {
        qDebug() << genresResponse.errorMessage;
        return;
    }

    const QVector<QString> genres = genresResponse.data;

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

    const QString username = UserSession::instance().getCurrentUser()->username;

    UserResponse saveResponse = UserService::saveInterestsGenre(username, selectedGenres);

    if (!saveResponse.success) {
        qDebug() << saveResponse.errorMessage;
        return;
    }

    UserSession::instance().setInterestsGenres(selectedGenres);

    this->hide();
    auto layout = new Layout();
    layout->show();
}

void InterestsDialog::on_skipButton_clicked() {
    const QString username = UserSession::instance().getCurrentUser()->username;

    UserResponse skipResponse = UserService::saveInterestsGenre(username, {});

    if (!skipResponse.success) {
        qDebug() << skipResponse.errorMessage;
        return;
    }

    this->hide();
    auto layout = new Layout();
    layout->show();
}
