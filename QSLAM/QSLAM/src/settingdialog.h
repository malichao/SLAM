#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings{
        int carScaleRatio;
    };

    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

    Settings setting()const;

private slots:
    void on_carScaleSpinBox_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::SettingDialog *ui;
    Settings currentSettings;
};

#endif // SETTINGDIALOG_H
