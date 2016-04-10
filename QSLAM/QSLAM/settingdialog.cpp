#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    currentSettings.carScaleRatio=300;
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

SettingDialog::Settings SettingDialog::setting() const{
    return currentSettings;
}

void SettingDialog::on_buttonBox_accepted()
{
    currentSettings.carScaleRatio=ui->carScaleSpinBox->value();
}
