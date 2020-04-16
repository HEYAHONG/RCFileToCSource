#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QList>
#include <QFile>
#include <QFileInfo>

typedef struct
{
QString Name;
QString Path;
} File_Info_T;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();

    void on_action_2_triggered();

    void on_lineEdit_name_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_delete_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_new_clicked();

    void on_action_C_triggered();

private:
    Ui::MainWindow *ui;
    QList<File_Info_T> fileinfo;
    //更新列表控件
    void update_list();
};
#endif // MAINWINDOW_H
