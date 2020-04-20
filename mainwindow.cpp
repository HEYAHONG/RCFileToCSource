#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("文件转C源文件");
    fileinfo.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{//关于菜单
    QMessageBox::information(this,"关于","RCFileToCSource BY 何亚红");
}

void MainWindow::on_action_2_triggered()
{
    close();
}

void MainWindow::on_lineEdit_name_textChanged(const QString &arg1)
{
    if(arg1.length()==0)
           ui->lineEdit_name->setText("RC"+QString::number(ui->listWidget->currentRow()>=0?ui->listWidget->currentRow():0));
}

void MainWindow::on_pushButton_clicked()
{//浏览文件
    QString path=QFileDialog::getOpenFileName(this,"浏览文件","","*");
    if(path.length()>0)
            ui->lineEdit_path->setText(path);
    ui->lineEdit_path->setToolTip(path);

}

//更新列表控件
void MainWindow::update_list()
{
    ui->listWidget->clear();
    size_t i=1;
    foreach(File_Info_T file,fileinfo)
    {
        ui->listWidget->addItem(QString::number(i)+"."+file.Name);
        i++;
    }
    ui->listWidget->setCurrentRow(fileinfo.size()-1);

}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow >=0 && currentRow< fileinfo.size())
    {
        ui->lineEdit_name->setText(fileinfo[currentRow].Name);
        ui->lineEdit_path->setText(fileinfo[currentRow].Path);
        ui->lineEdit_path->setToolTip(fileinfo[currentRow].Path);
    }
}

void MainWindow::on_pushButton_delete_clicked()
{
    if(ui->listWidget->currentRow()<0 || ui->listWidget->currentRow() >= fileinfo.size())
    {
        QMessageBox::warning(this,"警告","未选择有效项");
        return;
    }

    {//移动项
        for(int i=0;i+1<fileinfo.size();i++)
        {
            if(i>=ui->listWidget->currentRow())
            {//向前移动一项
                fileinfo[i].Name=fileinfo[i+1].Name;
                fileinfo[i].Path=fileinfo[i+1].Path;
            }
        }
    }

    {//释放队列末尾的数据
        if(fileinfo.size()>0)
            fileinfo.pop_back();
    }

    update_list();
}

void MainWindow::on_pushButton_save_clicked()
{
    if(ui->listWidget->currentRow()<0 || ui->listWidget->currentRow() >= fileinfo.size())
    {
        QMessageBox::warning(this,"警告","未选择有效项");
        return;
    }

    int i=ui->listWidget->currentRow();

    {//保存数据
        fileinfo[ui->listWidget->currentRow()].Name=ui->lineEdit_name->text();
        fileinfo[ui->listWidget->currentRow()].Path=ui->lineEdit_path->text();
    }

    update_list();

    ui->listWidget->setCurrentRow(i);
}


void MainWindow::on_pushButton_new_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"选择文件","");
    if(path.length()==0)
    {
        QMessageBox::warning(this,"警告","未选择文件");
        return;
    }

    QString name="";
    {
        QFile file;
        file.setFileName(path);
        QFileInfo info(file);
        name=info.fileName();
    }

    {//添加到fileinfo
        File_Info_T file;
        file.Name=name;
        file.Path=path;
        fileinfo.push_back(file);
    }

    update_list();
}

void MainWindow::on_action_C_triggered()
{//开始生成
    ui->statusbar->showMessage("开始生成C文件，目标目录的RC.h,RC.c将被覆盖!\n");

    {//检查列表中的文件
        if(fileinfo.size()==0)
        {
            QMessageBox::warning(this,"警告","列表中无文件，请使用新建添加资源文件!\n");
            return;
        }
    }

    QString dirpath=QFileDialog::getExistingDirectory(this,"选择C文件保存的目录:");
    if(dirpath.length()==0)
    {
        QMessageBox::warning(this,"警告","未选择输出的目录");
        return;
    }



    QDir current;
    current.setPath(dirpath);
    current.setCurrent(dirpath);//设置当前目录

    {
        QFile file1,file2;
        file1.setFileName(dirpath+"/RC.c");
        if(file1.exists())
        if(!QFile::remove(dirpath+"/RC.c"))
        {
            QMessageBox::warning(this,"警告","删除原有的C文件失败!");
            return;
        }



        file2.setFileName(dirpath+"/RC.h");
        if(file2.exists())
        if(!QFile::remove(dirpath+"/RC.h"))
        {
            QMessageBox::warning(this,"警告","删除原有的C文件失败!");
            return;
        }
    }

    {//输出RC.h文件
        QFile file,rc;
        file.setFileName(dirpath+"/RC.h");
        file.open(QFile::ReadWrite);
        rc.setFileName(":/qrc/RC.h");
        rc.open(QFile::ReadOnly);
        file.write(rc.readAll());
        rc.close();
        file.close();

    }

    {//输出RC.c文件
        QFile file,rc;
        file.setFileName(dirpath+"/RC.c");
        file.open(QFile::ReadWrite);
        rc.setFileName(":/qrc/RC.c");
        rc.open(QFile::ReadOnly);
        file.write(rc.readAll());

        file.write("\n\r");

        {//输出RC_Info_Size
            file.write((QString("const size_t    RC_Info_Size=")+QString::number(fileinfo.size())+";").toStdString().c_str());
        }

        file.write("\n\r");

        {//输出RC_Info
            file.write("const RC_Info_t RC_Info[]=\n\r{\n\r");
            size_t data_offset=0,name_offset=0;
            for(int i=0;i<fileinfo.size();i++)
            {
                QFile _file;
                _file.setFileName(fileinfo[i].Path);
                size_t data_size=_file.size();
                size_t name_size=fileinfo[i].Name.length()+1;

                //输出注释
                file.write((QString("//")+fileinfo[i].Path+"\n\r").toStdString().c_str());

                file.write((QString("{")+QString::number(data_offset)+","+QString::number(data_size)+","+QString::number(name_offset)+","+QString::number(name_size)+"}").toStdString().c_str());

                data_offset+=data_size;
                name_offset+=name_size;

                if(i<fileinfo.size()-1)
                {
                    file.write(",");
                }

                    file.write("\n\r");

            }

            file.write("};");

        }

        file.write("\n\r");

        {//输出RC_Data
            file.write("const unsigned char RC_Data[]=\n\r{\n\r");
            for(int i=0;i<fileinfo.size();i++)
            {
                QFile _file;
                _file.setFileName(fileinfo[i].Path);
                if(_file.open(QFile::ReadOnly))
                {
                    //输出注释
                    file.write((QString("//")+fileinfo[i].Path+"\n\r").toStdString().c_str());

                    QByteArray data=_file.read(16);
                    while(data.length()>0)
                    {
                        QByteArray _data=_file.read(16);
                        {//写入数据
                            for(int index=0;index<data.length();index++)
                            {
                                file.write((QString("0X")+QString::number(uint8_t(data[index]),16)).toStdString().c_str());
                                if(index<data.length()-1)
                                    file.write(",");
                                if(index==data.length()-1 && _data.length()>0)
                                    file.write(",");

                            }
                            file.write("\n\r");
                        }
                        data=_data;
                    }
                }
                else
                {
                    QMessageBox::warning(this,"警告","打开文件失败");
                }

                if(i<fileinfo.size()-1)
                {
                   file.write(",\n\r");
                }
            }
            file.write("};");
        }

        file.write("\n\r");
        {//输出RC_Name
            file.write("const unsigned char RC_Name[]=\n\r{\n\r");
            for(int i=0;i<fileinfo.size();i++)
            {

                //输出注释
                file.write((QString("//")+fileinfo[i].Path+"\n\r").toStdString().c_str());

                for(int j=0;j<fileinfo[i].Name.length();j++)
                {
                    file.write((QString("0X")+QString::number(uint8_t(fileinfo[i].Name.toStdString()[j]),16)).toStdString().c_str());
                    if(j<fileinfo[i].Name.length()-1)
                        file.write(",");
                }
                if(i<fileinfo.size()-1)
                {
                    file.write(",0X0,\n\r");
                }
                else
                {
                    file.write(",0x0\n\r");
                }
            }
            file.write("};");

        }
        file.write("\n\r");

        rc.close();
        file.close();

    }
}
