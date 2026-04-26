#include "auth.h"
#include <QString>
#include "authService.h"
#include "interestsDialog.h"
#include "layout.h"
#include "ui_auth.h"

Auth::Auth(QWidget *parent) : QWidget(parent), ui(new Ui::Auth) {
    ui->setupUi(this);

    ui->authStackedWidget->setCurrentIndex(SignIn);


    connect(ui->signin_usernameLineEdit, &QLineEdit::returnPressed, this, [this]() {
        ui->signin_passwordLineEdit->setFocus();
    });
    connect(ui->signin_passwordLineEdit, &QLineEdit::returnPressed, this, &Auth::on_signin_signInButton_clicked);


    connect(ui->signup_usernameLineEdit, &QLineEdit::returnPressed, this, [this]() {
        ui->signup_passwordLineEdit->setFocus();
    });
    connect(ui->signup_passwordLineEdit, &QLineEdit::returnPressed, this, [this]() {
        ui->signup_confirmPasswordLineEdit->setFocus();
    });
    connect(ui->signup_confirmPasswordLineEdit, &QLineEdit::returnPressed, this, &Auth::on_signup_signUpButton_clicked);
}

Auth::~Auth() {
    delete ui;
}

void Auth::on_signin_signInButton_clicked() {
    ui->signin_errorLabel->clear();

    const QString username = ui->signin_usernameLineEdit->text().trimmed();
    const QString password = ui->signin_passwordLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        ui->signin_errorLabel->setText("Please enter all fields.");
        return;
    }

    if (username.length() < 3) {
        ui->signin_errorLabel->setText("Username must be at least 3 characters long.");
        return;
    }

    if (username.length() > 20) {
        ui->signin_errorLabel->setText("Username must be less than 20 characters long.");
        return;
    }

    AuthResponse authResponse = AuthService::signIn(username, password);

    if (!authResponse.success) {
        ui->signin_errorLabel->setText(authResponse.errorMessage);
        return;
    }

    redirectToLayout();
}

void Auth::on_signup_signUpButton_clicked() {
    ui->signup_errorLabel->clear();

    const QString username = ui->signup_usernameLineEdit->text().trimmed();
    const QString password = ui->signup_passwordLineEdit->text().trimmed();
    const QString confirmPassword = ui->signup_confirmPasswordLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        ui->signup_errorLabel->setText("Please enter all fields.");
        return;
    }

    if (username.length() < 3) {
        ui->signup_errorLabel->setText("Username must be at least 3 characters long.");
        return;
    }

    if (username.length() > 20) {
        ui->signup_errorLabel->setText("Username must be less than 20 characters long.");
        return;
    }

    if (password != confirmPassword) {
        ui->signup_errorLabel->setText("Passwords should match.");
        return;
    }

    AuthResponse authResponse = AuthService::signUp(username, password);

    if (!authResponse.success) {
        ui->signup_errorLabel->setText(authResponse.errorMessage);
        return;
    }

    this->hide();
    auto interestsDialog = new InterestsDialog();
    interestsDialog->show();
}

void Auth::on_signin_switchButton_clicked() {
    ui->authStackedWidget->setCurrentIndex(SignUp);
}

void Auth::on_signup_switchButton_clicked() {
    ui->authStackedWidget->setCurrentIndex(SignIn);
}

void Auth::on_signin_backButton_clicked() {
    redirectToLayout();
}

void Auth::on_signup_backButton_clicked() {
    redirectToLayout();
}

void Auth::redirectToLayout() {
    this->hide();
    auto layout = new Layout();
    layout->show();
}
