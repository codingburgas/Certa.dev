#include "settings.h"

#include <QCheckBox>
#include <QMessageBox>
#include <QVector>

#include "auth.h"
#include "authService.h"
#include "movieService.h"
#include "ui_settings.h"
#include "userService.h"
#include "userSession.h"

Settings::Settings(QWidget *parent) : QWidget(parent), ui(new Ui::Settings) {
    ui->setupUi(this);
    ui->settingsStackedWidget->setCurrentWidget(ui->mainSettingsPage);

    loadGenres();
    refreshUserState();
}

Settings::~Settings() {
    delete ui;
}

void Settings::refreshUserState() {
    const auto currentUser = UserSession::instance().getCurrentUser();
    const bool isLoggedIn = currentUser.has_value();

    ui->settingsNoticeLabel->setText(isLoggedIn ? "" : "Sign in to manage your settings.");
    ui->usernameLineEdit->setText(isLoggedIn ? currentUser->username : "");

    ui->saveUsernameButton->setEnabled(isLoggedIn);
    ui->savePasswordButton->setEnabled(isLoggedIn);
    ui->saveInterestsButton->setEnabled(isLoggedIn);
    ui->deleteAccountButton->setEnabled(isLoggedIn);

    applyInterestsSelection();
}

void Settings::loadGenres() {
    MovieResponse genresResponse = MovieService::getAllGenres();

    clearGenresGrid();

    if (!genresResponse.success) {
        ui->interestsStatusLabel->setText(genresResponse.errorMessage);
        return;
    }

    const QVector<QString> genres = genresResponse.data;

    for (int i = 0; i < genres.size(); i++) {
        auto *genreCheckBox = new QCheckBox(genres[i], this);
        genreCheckBox->setMinimumHeight(34);
        ui->genresGridLayout->addWidget(genreCheckBox, i / 4, i % 4);
    }
}

void Settings::applyInterestsSelection() {
    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value()) {
        return;
    }

    QVector<QString> selected;
    for (const auto &genre : currentUser->interestsGenres) {
        selected.append(genre);
    }

    for (int i = 0; i < ui->genresGridLayout->count(); i++) {
        QLayoutItem *item = ui->genresGridLayout->itemAt(i);
        auto *checkBox = qobject_cast<QCheckBox *>(item ? item->widget() : nullptr);
        if (checkBox) {
            checkBox->setChecked(selected.contains(checkBox->text()));
        }
    }
}

void Settings::clearGenresGrid() {
    for (int i = ui->genresGridLayout->count() - 1; i >= 0; i--) {
        QLayoutItem *item = ui->genresGridLayout->takeAt(i);
        if (!item) {
            continue;
        }

        QWidget *widget = item->widget();
        if (widget) {
            widget->deleteLater();
        }

        delete item;
    }
}

void Settings::on_saveUsernameButton_clicked() {
    ui->usernameStatusLabel->clear();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value()) {
        ui->usernameStatusLabel->setText("Please sign in to update your username.");
        return;
    }

    const QString newUsername = ui->usernameLineEdit->text().trimmed();
    const QString currentUsername = currentUser->username;

    if (newUsername.isEmpty()) {
        ui->usernameStatusLabel->setText("Please enter a username.");
        return;
    }

    if (newUsername.length() < 3) {
        ui->usernameStatusLabel->setText("Username must be at least 3 characters long.");
        return;
    }

    if (newUsername.length() > 20) {
        ui->usernameStatusLabel->setText("Username must be less than 20 characters long.");
        return;
    }

    UserResponse updateResponse = UserService::updateUsername(currentUsername, newUsername);
    if (!updateResponse.success) {
        ui->usernameStatusLabel->setText(updateResponse.errorMessage);
        return;
    }

    User updatedUser = *currentUser;
    updatedUser.username = newUsername;
    UserSession::instance().setCurrentUser(updatedUser);

    ui->usernameStatusLabel->setText("Username updated.");
}

void Settings::on_savePasswordButton_clicked() {
    ui->passwordStatusLabel->clear();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value()) {
        ui->passwordStatusLabel->setText("Please sign in to update your password.");
        return;
    }

    const QString currentPassword = ui->currentPasswordLineEdit->text().trimmed();
    const QString newPassword = ui->newPasswordLineEdit->text().trimmed();
    const QString confirmPassword = ui->confirmPasswordLineEdit->text().trimmed();

    if (currentPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        ui->passwordStatusLabel->setText("Please enter all password fields.");
        return;
    }

    if (newPassword != confirmPassword) {
        ui->passwordStatusLabel->setText("New passwords should match.");
        return;
    }

    UserResponse updateResponse = UserService::changePassword(currentUser->username, currentPassword, newPassword);
    if (!updateResponse.success) {
        ui->passwordStatusLabel->setText(updateResponse.errorMessage);
        return;
    }

    ui->currentPasswordLineEdit->clear();
    ui->newPasswordLineEdit->clear();
    ui->confirmPasswordLineEdit->clear();
    ui->passwordStatusLabel->setText("Password updated.");
}

void Settings::on_saveInterestsButton_clicked() {
    ui->interestsStatusLabel->clear();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value()) {
        ui->interestsStatusLabel->setText("Please sign in to update your interests.");
        return;
    }

    QVector<QString> selectedGenres;
    for (int i = 0; i < ui->genresGridLayout->count(); i++) {
        QLayoutItem *item = ui->genresGridLayout->itemAt(i);
        auto *checkBox = qobject_cast<QCheckBox *>(item ? item->widget() : nullptr);
        if (checkBox && checkBox->isChecked()) {
            selectedGenres.append(checkBox->text());
        }
    }

    UserResponse saveResponse = UserService::saveInterestsGenre(currentUser->username, selectedGenres);
    if (!saveResponse.success) {
        ui->interestsStatusLabel->setText(saveResponse.errorMessage);
        return;
    }

    UserSession::instance().setInterestsGenres(selectedGenres);
    ui->interestsStatusLabel->setText("Interests updated.");
}

void Settings::on_termsViewButton_clicked() {
    ui->settingsStackedWidget->setCurrentWidget(ui->termsPage);
}

void Settings::on_privacyViewButton_clicked() {
    ui->settingsStackedWidget->setCurrentWidget(ui->privacyPage);
}

void Settings::on_termsBackButton_clicked() {
    ui->settingsStackedWidget->setCurrentWidget(ui->mainSettingsPage);
}

void Settings::on_privacyBackButton_clicked() {
    ui->settingsStackedWidget->setCurrentWidget(ui->mainSettingsPage);
}

void Settings::on_deleteAccountButton_clicked() {
    ui->deleteStatusLabel->clear();

    const auto currentUser = UserSession::instance().getCurrentUser();
    if (!currentUser.has_value()) {
        ui->deleteStatusLabel->setText("Please sign in to delete your account.");
        return;
    }

    QMessageBox confirmation(this);
    confirmation.setIcon(QMessageBox::Warning);
    confirmation.setWindowTitle("Delete account");
    confirmation.setText("Are you sure you want to delete your account? This action is irreversible.");
    confirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmation.setDefaultButton(QMessageBox::No);

    if (confirmation.exec() != QMessageBox::Yes) {
        return;
    }

    UserResponse deleteResponse = UserService::deleteUser(currentUser->username);
    if (!deleteResponse.success) {
        ui->deleteStatusLabel->setText(deleteResponse.errorMessage);
        return;
    }

    AuthService::logout();

    QWidget *window = this->window();
    if (window) {
        window->hide();
    }

    auto authPage = new Auth();
    authPage->show();
}
