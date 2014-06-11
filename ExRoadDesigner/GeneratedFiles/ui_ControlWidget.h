/********************************************************************************
** Form generated from reading UI file 'ControlWidget.ui'
**
** Created: Wed Jun 11 17:38:05 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLWIDGET_H
#define UI_CONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlWidget
{
public:
    QWidget *dockWidgetContents;
    QGroupBox *groupBox_6;
    QLabel *terrainPaint_sizeLabel;
    QLabel *terrainPaint_changeLabel;
    QSlider *terrainPaint_sizeSlider;
    QSlider *terrainPaint_changeSlider;
    QCheckBox *terrain_2DShader;
    QLabel *render_2DroadsStrokeLabel;
    QSlider *render_2DroadsStrokeSlider;
    QSlider *render_2DroadsExtraWidthSlider;
    QLabel *render_2DroadsExtraWidthLabel;
    QLabel *render_2DparksLabel;
    QSlider *render_2DparksSlider;
    QCheckBox *render_3DtreesCheckBox;
    QLabel *render_2DparcelLineLabel;
    QSlider *render_2DparcelLineSlider;
    QLineEdit *lineEditMaxBlockSizeForPark;
    QLabel *label_15;

    void setupUi(QDockWidget *ControlWidget)
    {
        if (ControlWidget->objectName().isEmpty())
            ControlWidget->setObjectName(QString::fromUtf8("ControlWidget"));
        ControlWidget->resize(190, 265);
        ControlWidget->setMinimumSize(QSize(190, 240));
        ControlWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        groupBox_6 = new QGroupBox(dockWidgetContents);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 10, 171, 221));
        terrainPaint_sizeLabel = new QLabel(groupBox_6);
        terrainPaint_sizeLabel->setObjectName(QString::fromUtf8("terrainPaint_sizeLabel"));
        terrainPaint_sizeLabel->setGeometry(QRect(10, 20, 61, 16));
        terrainPaint_changeLabel = new QLabel(groupBox_6);
        terrainPaint_changeLabel->setObjectName(QString::fromUtf8("terrainPaint_changeLabel"));
        terrainPaint_changeLabel->setGeometry(QRect(10, 40, 61, 16));
        terrainPaint_sizeSlider = new QSlider(groupBox_6);
        terrainPaint_sizeSlider->setObjectName(QString::fromUtf8("terrainPaint_sizeSlider"));
        terrainPaint_sizeSlider->setGeometry(QRect(80, 20, 81, 15));
        terrainPaint_sizeSlider->setMaximumSize(QSize(16777215, 17));
        terrainPaint_sizeSlider->setMinimum(1);
        terrainPaint_sizeSlider->setMaximum(25);
        terrainPaint_sizeSlider->setValue(5);
        terrainPaint_sizeSlider->setOrientation(Qt::Horizontal);
        terrainPaint_changeSlider = new QSlider(groupBox_6);
        terrainPaint_changeSlider->setObjectName(QString::fromUtf8("terrainPaint_changeSlider"));
        terrainPaint_changeSlider->setGeometry(QRect(80, 40, 81, 17));
        terrainPaint_changeSlider->setMaximumSize(QSize(16777215, 17));
        terrainPaint_changeSlider->setMinimum(1);
        terrainPaint_changeSlider->setValue(10);
        terrainPaint_changeSlider->setOrientation(Qt::Horizontal);
        terrain_2DShader = new QCheckBox(groupBox_6);
        terrain_2DShader->setObjectName(QString::fromUtf8("terrain_2DShader"));
        terrain_2DShader->setGeometry(QRect(10, 60, 111, 20));
        terrain_2DShader->setChecked(true);
        render_2DroadsStrokeLabel = new QLabel(groupBox_6);
        render_2DroadsStrokeLabel->setObjectName(QString::fromUtf8("render_2DroadsStrokeLabel"));
        render_2DroadsStrokeLabel->setGeometry(QRect(10, 120, 71, 16));
        render_2DroadsStrokeSlider = new QSlider(groupBox_6);
        render_2DroadsStrokeSlider->setObjectName(QString::fromUtf8("render_2DroadsStrokeSlider"));
        render_2DroadsStrokeSlider->setGeometry(QRect(80, 120, 81, 17));
        render_2DroadsStrokeSlider->setMaximumSize(QSize(16777215, 17));
        render_2DroadsStrokeSlider->setMaximum(100);
        render_2DroadsStrokeSlider->setValue(35);
        render_2DroadsStrokeSlider->setOrientation(Qt::Horizontal);
        render_2DroadsExtraWidthSlider = new QSlider(groupBox_6);
        render_2DroadsExtraWidthSlider->setObjectName(QString::fromUtf8("render_2DroadsExtraWidthSlider"));
        render_2DroadsExtraWidthSlider->setGeometry(QRect(90, 100, 71, 17));
        render_2DroadsExtraWidthSlider->setMaximumSize(QSize(16777215, 17));
        render_2DroadsExtraWidthSlider->setMaximum(100);
        render_2DroadsExtraWidthSlider->setOrientation(Qt::Horizontal);
        render_2DroadsExtraWidthLabel = new QLabel(groupBox_6);
        render_2DroadsExtraWidthLabel->setObjectName(QString::fromUtf8("render_2DroadsExtraWidthLabel"));
        render_2DroadsExtraWidthLabel->setGeometry(QRect(10, 100, 71, 16));
        render_2DparksLabel = new QLabel(groupBox_6);
        render_2DparksLabel->setObjectName(QString::fromUtf8("render_2DparksLabel"));
        render_2DparksLabel->setGeometry(QRect(10, 150, 61, 16));
        render_2DparksSlider = new QSlider(groupBox_6);
        render_2DparksSlider->setObjectName(QString::fromUtf8("render_2DparksSlider"));
        render_2DparksSlider->setGeometry(QRect(80, 150, 81, 17));
        render_2DparksSlider->setMaximumSize(QSize(16777215, 17));
        render_2DparksSlider->setValue(10);
        render_2DparksSlider->setOrientation(Qt::Horizontal);
        render_3DtreesCheckBox = new QCheckBox(groupBox_6);
        render_3DtreesCheckBox->setObjectName(QString::fromUtf8("render_3DtreesCheckBox"));
        render_3DtreesCheckBox->setGeometry(QRect(10, 80, 101, 20));
        render_3DtreesCheckBox->setChecked(true);
        render_2DparcelLineLabel = new QLabel(groupBox_6);
        render_2DparcelLineLabel->setObjectName(QString::fromUtf8("render_2DparcelLineLabel"));
        render_2DparcelLineLabel->setGeometry(QRect(10, 190, 81, 16));
        render_2DparcelLineSlider = new QSlider(groupBox_6);
        render_2DparcelLineSlider->setObjectName(QString::fromUtf8("render_2DparcelLineSlider"));
        render_2DparcelLineSlider->setGeometry(QRect(100, 190, 61, 17));
        render_2DparcelLineSlider->setMaximumSize(QSize(16777215, 17));
        render_2DparcelLineSlider->setMaximum(100);
        render_2DparcelLineSlider->setValue(20);
        render_2DparcelLineSlider->setOrientation(Qt::Horizontal);
        lineEditMaxBlockSizeForPark = new QLineEdit(groupBox_6);
        lineEditMaxBlockSizeForPark->setObjectName(QString::fromUtf8("lineEditMaxBlockSizeForPark"));
        lineEditMaxBlockSizeForPark->setGeometry(QRect(82, 170, 81, 20));
        label_15 = new QLabel(groupBox_6);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(50, 170, 31, 21));
        ControlWidget->setWidget(dockWidgetContents);
        QWidget::setTabOrder(terrainPaint_sizeSlider, terrainPaint_changeSlider);
        QWidget::setTabOrder(terrainPaint_changeSlider, terrain_2DShader);
        QWidget::setTabOrder(terrain_2DShader, render_3DtreesCheckBox);
        QWidget::setTabOrder(render_3DtreesCheckBox, render_2DroadsExtraWidthSlider);
        QWidget::setTabOrder(render_2DroadsExtraWidthSlider, render_2DroadsStrokeSlider);
        QWidget::setTabOrder(render_2DroadsStrokeSlider, render_2DparksSlider);
        QWidget::setTabOrder(render_2DparksSlider, render_2DparcelLineSlider);

        retranslateUi(ControlWidget);

        QMetaObject::connectSlotsByName(ControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ControlWidget)
    {
        groupBox_6->setTitle(QApplication::translate("ControlWidget", "Terrain Controls/Render", 0, QApplication::UnicodeUTF8));
        terrainPaint_sizeLabel->setText(QApplication::translate("ControlWidget", "Size: 5%", 0, QApplication::UnicodeUTF8));
        terrainPaint_changeLabel->setText(QApplication::translate("ControlWidget", "Change", 0, QApplication::UnicodeUTF8));
        terrain_2DShader->setText(QApplication::translate("ControlWidget", "2D Shader", 0, QApplication::UnicodeUTF8));
        render_2DroadsStrokeLabel->setText(QApplication::translate("ControlWidget", "Stroke: 3.5", 0, QApplication::UnicodeUTF8));
        render_2DroadsExtraWidthLabel->setText(QApplication::translate("ControlWidget", "R Width: 0.0", 0, QApplication::UnicodeUTF8));
        render_2DparksLabel->setText(QApplication::translate("ControlWidget", "Park 10 %", 0, QApplication::UnicodeUTF8));
        render_3DtreesCheckBox->setText(QApplication::translate("ControlWidget", "3D Trees", 0, QApplication::UnicodeUTF8));
        render_2DparcelLineLabel->setText(QApplication::translate("ControlWidget", "Par. Line: 2.0", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("ControlWidget", "Size:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ControlWidget);
    } // retranslateUi

};

namespace Ui {
    class ControlWidget: public Ui_ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
