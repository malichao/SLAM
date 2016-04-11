/********************************************************************************
** Form generated from reading UI file 'settingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGDIALOG_H
#define UI_SETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *carScaleLabel;
    QSpinBox *carScaleSpinBox;

    void setupUi(QDialog *SettingDialog)
    {
        if (SettingDialog->objectName().isEmpty())
            SettingDialog->setObjectName(QStringLiteral("SettingDialog"));
        SettingDialog->resize(509, 531);
        buttonBox = new QDialogButtonBox(SettingDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(150, 480, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget = new QWidget(SettingDialog);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(100, 190, 177, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        carScaleLabel = new QLabel(horizontalLayoutWidget);
        carScaleLabel->setObjectName(QStringLiteral("carScaleLabel"));

        horizontalLayout->addWidget(carScaleLabel);

        carScaleSpinBox = new QSpinBox(horizontalLayoutWidget);
        carScaleSpinBox->setObjectName(QStringLiteral("carScaleSpinBox"));
        carScaleSpinBox->setMinimum(10);
        carScaleSpinBox->setMaximum(500);
        carScaleSpinBox->setSingleStep(10);
        carScaleSpinBox->setValue(300);

        horizontalLayout->addWidget(carScaleSpinBox);


        retranslateUi(SettingDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SettingDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingDialog)
    {
        SettingDialog->setWindowTitle(QApplication::translate("SettingDialog", "Dialog", 0));
        carScaleLabel->setText(QApplication::translate("SettingDialog", "Car Scale Ratio:", 0));
    } // retranslateUi

};

namespace Ui {
    class SettingDialog: public Ui_SettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGDIALOG_H
