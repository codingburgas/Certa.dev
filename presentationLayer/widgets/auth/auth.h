#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class Auth;
}

QT_END_NAMESPACE

class Auth : public QWidget {
    Q_OBJECT

public:
    explicit Auth(QWidget *parent = nullptr);

    ~Auth() override;

private slots:
    void on_signin_signInButton_clicked();

    void on_signup_signUpButton_clicked();

    void on_signin_switchButton_clicked();

    void on_signup_switchButton_clicked();

    void on_signin_backButton_clicked();

    void on_signup_backButton_clicked();

private:
    Ui::Auth *ui;

    void redirectToLayout();
};
